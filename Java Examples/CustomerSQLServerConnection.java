/**
 * The given extract is from 'Apprentice' human resource management software, written by Rohit Saini & Raj Saini at
 * Twin Leaf Entertainment LLC. All code is subject to copyright.
 * 
 * Purpose: Customer remote connection to SQL database, and check if they have paid for their subscription.
 * If not, the return value can be used to restrict access to the software.
 */

package files;

//standard SQL imports
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class CustomerServer {

	//SQL database connection
	static Connection connection;

	//for debugging
	static String prefix = "[CustomerServer] ";

	public static Connection connection() {

		if (connection != null)
			return connection;

		else System.out.println(prefix + "THE CONNECTION IS NULL");

		try {

			//select driver
			Class.forName("com.mysql.jdbc.Driver");

			//determine remote host, username and password
			//Note that these are removed as this is sample code.
			String host = "jdbc:mysql://address/database"; //placeholder so server cannot be accessed from this example
			String username = "username";
			String password = "password";

			//establish a connection
			Connection connect = DriverManager.getConnection(host, username, password);

			connection = connect;

			return connect;

		} catch (ClassNotFoundException e) {

			System.out.println(prefix+"Error : " + e.getMessage());

		} catch (Exception e) {

			System.out.println(prefix+"Exception: " + e.getMessage());
		} 

		return null;
	}
	
	//fetch all logged data/operations from the server
		public static boolean getActivationStatus(String company) {
			
			boolean activated = false;
			
			testConnection();
			
			try {
				
				Statement st = connection().createStatement();

				ResultSet rs = st.executeQuery("SELECT * FROM  payments");
				
				//get all data strings to check if customer has paid their bill
				while (rs.next()) {
					
					String company_name = rs.getString("company_name");
					
					if (company_name.equalsIgnoreCase(company)) {
					
						String status = rs.getString("status");
						
						//check activation status and if they paid
						//'W' value is specific to back-end processing
						if (status.equals("W")) {
							activated = true;
							System.out.println(prefix+"All payments have been made.");
						} 
					}
				}
			
			} catch (SQLException e) {
				System.out.println(prefix+"Error occurred in fetching data: "+e.getMessage());
			}
			
			return activated;
		}
		
		//connection test before reading database values
		public static boolean testConnection() {

			try {
				final Statement st = connection().createStatement();
				ResultSet rs = st.executeQuery("SELECT 1 FROM  files");
				rs.close();
				System.out.println(prefix+"Connection is valid");

			} catch (SQLException | NullPointerException e) {
				// handle SQL error
				System.out.println(prefix+"The connection IS NO LONGER VALID. Resetting connection to null for next request.");
				connection = null;

			}
			return true;
		}
}
