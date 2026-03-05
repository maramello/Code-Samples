#region Using declarations
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Xml.Serialization;
using NinjaTrader.Cbi;
using NinjaTrader.Gui;
using NinjaTrader.Gui.Chart;
using NinjaTrader.Gui.SuperDom;
using NinjaTrader.Gui.Tools;
using NinjaTrader.Data;
using NinjaTrader.NinjaScript;
using NinjaTrader.Core.FloatingPoint;
using NinjaTrader.NinjaScript.Indicators;
using NinjaTrader.NinjaScript.DrawingTools;
#endregion

// NOTE: script has been partially shown

namespace NinjaTrader.NinjaScript.Strategies
{
	public class NewsTrader : Strategy
	{
		private Order buyStopOrder;
		private Order sellStopOrder;
		private bool ordersPlaced;
		
		// target order
		private Order exitOrder;
		private bool exitOrderPlaced;
		
		// breakeven stop order for protection from bottom and top tick edging
		private Order breakevenOrder;
		private bool breakevenPlaced;

		protected override void OnStateChange()
		{
			if (State == State.SetDefaults)
			{
				Description = @"Place bracket orders before news release with automatic stop loss and profit target.";
				Name = "NewsFullporter";
				
				// Allow unmanaged orders
				IsUnmanaged = true;
				
				Calculate = Calculate.OnEachTick;
				EntriesPerDirection = 1;
				EntryHandling = EntryHandling.AllEntries;
				IsExitOnSessionCloseStrategy = true;
				ExitOnSessionCloseSeconds = 30;
				IsFillLimitOnTouch = false;
				MaximumBarsLookBack = MaximumBarsLookBack.TwoHundredFiftySix;
				OrderFillResolution = OrderFillResolution.Standard;
				StartBehavior = StartBehavior.WaitUntilFlat;
				TimeInForce = TimeInForce.Gtc;
				TraceOrders = false;
				RealtimeErrorHandling = RealtimeErrorHandling.StopCancelClose;
				StopTargetHandling = StopTargetHandling.PerEntryExecution;
				BarsRequiredToTrade = 20;
				IsInstantiatedOnEachOptimizationIteration = true;

				NumContracts = 1;
				NewsTime = DateTime.Parse("08:30", System.Globalization.CultureInfo.InvariantCulture);
				SecondsBeforeEntry = 60;
				BufferTicks = 80;
				BreakEvenPercent = 90;
				TakeProfitTicks = 120;
				
			}
			else if (State == State.Configure)
			{	
				// Allow execution per-second to check for entry order
				AddDataSeries(BarsPeriodType.Second, 1);
			}
		}
		
		// Text shown at top of chart when you add the strategy, can also be blank to have no text shown
		public override string DisplayName
		{
		  get { return "News Fullporter"; }
		}

		protected override void OnBarUpdate()
		{
			if (CurrentBar < BarsRequiredToTrade)
				return;
			
			// Only need to use 1 second timeframe not chart
			if (BarsInProgress == 0)
				return;

			TimeSpan currentTime = Time[0].TimeOfDay;
			TimeSpan newsTimeOfDay = NewsTime.TimeOfDay;
			TimeSpan entryWindowStart = newsTimeOfDay.Subtract(TimeSpan.FromSeconds(SecondsBeforeEntry));

			if (Bars.IsFirstBarOfSession && Position.MarketPosition == MarketPosition.Flat)
			{
				ordersPlaced = false;
				exitOrderPlaced = false;
				breakevenPlaced = false;
				
				buyStopOrder = null;
				sellStopOrder = null;
				breakevenOrder = null;
				
				Print($"[{Time[0]}] Session reset - fullporter ready for new orders");
			}

			bool inEntryWindow = currentTime >= entryWindowStart && currentTime < newsTimeOfDay;

			// Debug logging around the entry window, uncomment for debug
			if (currentTime >= entryWindowStart.Subtract(TimeSpan.FromSeconds(5)) && currentTime <= newsTimeOfDay.Add(TimeSpan.FromSeconds(5)))
			{
				// Print($"[{Time[0]}] Time: {currentTime}, Window: {entryWindowStart} to {newsTimeOfDay}, InWindow: {inEntryWindow}, Placed: {ordersPlaced}, Pos: {Position.MarketPosition}");
			}

			if (Position.MarketPosition == MarketPosition.Flat && !ordersPlaced && inEntryWindow)
			{
				double buyStopPrice = Close[0] + BufferTicks * TickSize;
				double sellStopPrice = Close[0] - BufferTicks * TickSize;

				Print($"[{Time[0]}] *** PLACING ORDERS ***");
				Print($"[{Time[0]}] BuyStop @ {buyStopPrice}, SellStop @ {sellStopPrice}");

				// Submit Buy Stop above current price
                buyStopOrder = SubmitOrderUnmanaged(0, OrderAction.Buy, OrderType.StopMarket, NumContracts, 0, buyStopPrice, $"BuyStop{CurrentBar}");
                // Submit Sell Stop below current price
                sellStopOrder = SubmitOrderUnmanaged(0, OrderAction.Sell, OrderType.StopMarket, NumContracts, 0, sellStopPrice, $"SellStop{CurrentBar}");
				
				ordersPlaced = true;
			}
			
			// Add breakeven stop order
			if (!breakevenPlaced && BreakEvenPercent > 0 && BreakEvenPercent < 100)
			{	
				// Calculate the number of ticks required to reach breakeven based on BEAfterProfitPercent
				double TicksToPassForBE = TakeProfitTicks * (BreakEvenPercent / 100.0);
				
				if (Position.MarketPosition == MarketPosition.Long)
				{
					if (buyStopOrder == null)
					{
						Print("In long position but buy stop entry order is null, this is most likely an error");
						return;
					}
					
					double UnrealizedTicks = (Close[0] - buyStopOrder.AverageFillPrice) * TickSize;
					
					// Calculate the price level that corresponds to the breakeven threshold
					double PriceToPassForBE = buyStopOrder.AverageFillPrice + TicksToPassForBE * TickSize;
					
					if (UnrealizedTicks > 0 && Close[0] > PriceToPassForBE)
					{
						// Move stop to entry price (break-even)
						breakevenOrder = SubmitOrderUnmanaged(0, OrderAction.Sell, OrderType.StopMarket, NumContracts, 0, buyStopOrder.AverageFillPrice, $"BuyBreakevenStop{CurrentBar}");
						breakevenPlaced = true;
					}
				}
				// Short
				else if (Position.MarketPosition == MarketPosition.Short)
				{
					if (sellStopOrder == null)
					{
						Print("In short position but short stop entry order is null, this is most likely an error");
						return;
					}
					
					double UnrealizedTicks = (sellStopOrder.AverageFillPrice - Close[0]) * TickSize;
					
					// Calculate the price level that corresponds to the breakeven threshold
					double PriceToPassForBE = sellStopOrder.AverageFillPrice - TicksToPassForBE * TickSize;
					
					if (UnrealizedTicks > 0 && Close[0] < PriceToPassForBE)
					{
						// Move stop to entry price (break-even)
						breakevenOrder = SubmitOrderUnmanaged(0, OrderAction.BuyToCover, OrderType.StopMarket, NumContracts, 0, sellStopOrder.AverageFillPrice, $"SellBreakevenStop{CurrentBar}");
						breakevenPlaced = true;
					}
				}
			}
		}
		
		protected override void OnOrderUpdate(Order order, double limitPrice, double stopPrice, int quantity, int filled, double averageFillPrice, 
	    OrderState orderState, DateTime time, ErrorCode errorCode, string oco)
		{
		    // Check if this is our order and it is filled
		    if (buyStopOrder != null && order == buyStopOrder)
		    {
		        if (orderState == OrderState.Filled)
		        {
		            // The fill price is here
		            Print("Order filled at: " + averageFillPrice);
		            
		            // Cancel other direction order
					if (sellStopOrder != null)
						CancelOrder(sellStopOrder);
					
					if (!exitOrderPlaced)
					{
						exitOrder = SubmitOrderUnmanaged(0, OrderAction.Sell, OrderType.Limit, NumContracts, averageFillPrice + TakeProfitTicks * TickSize, 0, $"TargetLimit{CurrentBar}");
						exitOrderPlaced = true;
						Print($"[{Time[0]}] TargetLimit placed @ {averageFillPrice + TakeProfitTicks * TickSize}");
					}
		        }
		    }
			else if (sellStopOrder != null && order == sellStopOrder)
		    {
		        if (orderState == OrderState.Filled)
		        {
		            // The fill price is here
		            Print("Order filled at: " + averageFillPrice);
		            
		            // Cancel other direction order
					if (buyStopOrder != null)
						CancelOrder(buyStopOrder);
					
					if (!exitOrderPlaced)
					{
						exitOrder = SubmitOrderUnmanaged(0, OrderAction.BuyToCover, OrderType.Limit, NumContracts, averageFillPrice - TakeProfitTicks * TickSize, 0, $"TargetLimit{CurrentBar}");
						exitOrderPlaced = true;
						Print($"[{Time[0]}] TargetLimit placed @ {averageFillPrice - TakeProfitTicks * TickSize}");
					}
		        }
		    }
			
			// If exit order is filled cancel breakeven stop (if one exists)
			else if (exitOrder != null && order == exitOrder && orderState == OrderState.Filled)
			{
				if (breakevenOrder != null)
				{
					Print("Canceling breakeven order due to target being hit");
					CancelOrder(breakevenOrder);
				}
			}
			
			// Cancel target order if breakeven gets hit
			else if (breakevenOrder != null && order == breakevenOrder && orderState == OrderState.Filled)
			{
				if (exitOrder != null)
				{
					Print("Canceling target order due to breakeven being hit");
					CancelOrder(exitOrder);
				}
			}
		}

		#region Properties
		[NinjaScriptProperty]
		[Range(1, int.MaxValue)]
		[Display(Name = "Num Contracts", Order = 1, GroupName = "Parameters")]
		public int NumContracts { get; set; }

		[NinjaScriptProperty]
		[PropertyEditor("NinjaTrader.Gui.Tools.TimeEditorKey")]
		[Display(Name = "News Time", Order = 2, GroupName = "Parameters")]
		public DateTime NewsTime { get; set; }

		[NinjaScriptProperty]
		[Range(1, int.MaxValue)]
		[Display(Name = "Seconds Before Entry", Order = 3, GroupName = "Parameters")]
		public int SecondsBeforeEntry { get; set; }

		[NinjaScriptProperty]
		[Range(1, int.MaxValue)]
		[Display(Name = "Buffer Ticks", Description = "Ticks away from price for entry orders", Order = 4, GroupName = "Parameters")]
		public int BufferTicks { get; set; }

		[NinjaScriptProperty]
		[Range(0, 99)]
		[Display(Name = "BreakEvenPercent", Order = 5, GroupName = "Parameters")]
		public int BreakEvenPercent { get; set; }

		[NinjaScriptProperty]
		[Range(1, int.MaxValue)]
		[Display(Name = "Take Profit Ticks", Order = 6, GroupName = "Parameters")]
		public int TakeProfitTicks { get; set; }
		#endregion
	}
}
