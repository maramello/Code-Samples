/**
 * =====================================================
 *  Author: Rohit Saini
 * Purpose: Provides functionality for creating and
 *          validating multiple users through requests.
 *    Date: 11/2022
 * ======================================================
 */

//Import packages, installed separarely using npm
const base64 = require('base64url');
const uuid4 = require('uuid4');


//To store the active users
var activeUsers = [];

//Helper function to check for duplicate user
async function IsDuplicateUser(user) {

    //Assume no duplicate
    let dup = false;

    //Search method (search by ID)
    const search = async => {

        //Loop through and check if the user exists
        for (var i = 0; i < activeUsers.length; i++) {

            if (user.id === activeUsers[i].id) {
                dup = true;
            }
        }
    }

    await search();

    //Return after search
    return dup;
}

//Helper function to validate user session and whether it exists or not 
async function IsValidUser(username, id, session) {

    //Populate response fields (will be returned by this function for providing information to calling function)
    //Note that 'found' will be marked true if the user exists, and 'index' can be used to access the user in the active users array.
    let response = { found: false, password: '', index: 0, isValidSession: false, foundSession: false };

    //Async method to call
    const search = async => {

        //Check if session is valid and update response field if so
        if (uuid4.valid(session) === true) {
            response.isValidSession = true;
        }

        //Loop through all current users
        for (var i = 0; i < activeUsers.length; i++) {

            //If username or ID is found, mark the user as found and assign appropriate values
            if ((username === activeUsers[i].username) || (id === activeUsers[i].id)) {
                response.found = true;
                response.password = activeUsers[i].password;
                response.index = i;
            }

            //Mark session as found if it exists
            if (session === activeUsers[i].session)
                response.foundSession = true;
        }
    }

    await search();

    //Return populated response
    return response;
}

//Function to Create User 
async function CreateUser(req, res) {

    //This is the design of the user object, populating the fields here
    let user = {
        username: req.body['username'],
        password: req.body['password'],
        avatar: req.body['avatar'],
        id: base64.default.encode(req.body['username'])
    };

    //Call duplicate check function
    let dup = await IsDuplicateUser(user);

    //Send duplicate error if invalid
    if (dup === true) {
        res.status(409).json({ error: 'duplicate user' });
    }

    //Otherwise push valid users to current user list
    else {
        activeUsers.push(user);
        res.status(200).json(user);
    }
};

//Function to login
async function Login(req, res) {

    //Validate provided fields (only username here)
    let loginRes = await IsValidUser(req.body['username'], '', '');

    //Make sure the username provided is valid and a user was found
    if (loginRes.found === true) {

        //Validate corresponding password 
        if (loginRes.password === req.body['password']) {

            //create session ID
            let session = uuid4();

            //assign session to the user
            activeUsers[loginRes.index].session = session;

            //Return the session
            res.status(200).json({ session });
        }
        //Reaching here means the password was incorrect
        else {
            res.status(403).json({ error: 'password for login is incorrect' });
        }
    }
    //Reaching here means username is incorrect
    else {
        res.status(400).json({ error: 'username for login is incorrect' });
    }

};

//Get user data
async function GetUser(req, res) {

    //Variable to validate user information
    var checkRes;

    //Check if username was sent by query, and validate by username if so
    if (req.query['username']) {
        checkRes = await IsValidUser(req.query['username'], '', req.body['session']);
    }
    //Otherwise validate by ID
    else {
        checkRes = await IsValidUser('', req.params.id, req.body['session']);
    }

    //Invalid session
    if (checkRes.isValidSession === false) {
        res.status(401).json({ error: 'invalid session' });
        return;
    }

    //This if statement means valid session, but no matching user was found
    else if (checkRes.found === false) {

        //check if ID was provided and was invalid
        if (req.params.id) {
            res.status(404).json({ error: 'bad ID, but valid session' });
        }

        //Username was invalid
        else if (req.query['username']) {
            res.status(404).json({ error: 'invalid username entered' });
        }

        //Username was not provided at all
        else {
            res.status(400).json({ error: 'no username entered' });
        }
        return;
    }

    //if no user has the session, that means its an old session
    if (checkRes.isValidSession === true && checkRes.foundSession === false) {
        res.status(401).json({ error: 'old session' });
        return;
    }

    //good session and good ID found
    if (checkRes.found === true) {

        let foundUser = activeUsers[checkRes.index];

        //check if user is different from current
        if (activeUsers[checkRes.index].session !== req.body['session']) {

            //remove password field if it is different
            delete activeUsers[checkRes.index].password;
        }

        //send user data
        res.status(200).json(foundUser);

    } else {
        //this means invalid ID
        res.status(404).json({ error: 'invalid ID' });
    }

};

//Update user data from PUT
async function UpdateUser(req, res) {

    //if no session found, do not update
    if (!req.body['session']) {
        res.status(401).json({ error: 'no session found for updating user' });
        return;
    }

    //Variable to validate user information, pass ID and session
    var checkRes = await IsValidUser('', req.params.id, req.body['session']);

    //if bad session, return
    if (checkRes.isValidSession === false) {
        res.status(401).json({ error: 'bad session found for updating user' });
        return;
    }

    //Good session and good ID found
    if (checkRes.found === true) {

        //Check if user is different from current
        if (activeUsers[checkRes.index].session !== req.body['session']) {

            res.status(403).json({ error: 'cannot update user other than current' });
            return;
        }

        //Update required fields if they exist
        activeUsers[checkRes.index].session = req.body['session'];
        activeUsers[checkRes.index].username = req.body['username'];
        activeUsers[checkRes.index].password = req.body['password'];
        activeUsers[checkRes.index].avatar = req.body['avatar'];

        let updatedUser = activeUsers[checkRes.index];

        //send updated user data
        res.status(200).json(updatedUser);

    }
    //This means invalid ID or username
    else {
        res.status(404).json({ error: 'invalid ID' });
    }

};

//Export functions for use in other file
module.exports = { CreateUser, Login, GetUser, UpdateUser };
