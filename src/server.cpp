#define WIN32_LEAN_AND_MEAN

#include "httplib.h"
#include "SocialNetwork.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;


// ------------------------------------------------------------
// HTML ESCAPE FUNCTION
// ------------------------------------------------------------

string escapeHTML(const string& text)
{
    string result;

    for (char c : text)
    {
        switch (c)
        {
            case '&':
                result += "&amp;";
                break;

            case '<':
                result += "&lt;";
                break;

            case '>':
                result += "&gt;";
                break;

            case '"':
                result += "&quot;";
                break;

            case '\'':
                result += "&#39;";
                break;

            default:
                result += c;
        }
    }

    return result;
}


// ------------------------------------------------------------
// CREATE MAIN PAGE
// ------------------------------------------------------------

string createHomePage()
{
    string html = R"HTML(
<!DOCTYPE html>
<html>

<head>

<meta charset="UTF-8">

<meta name="viewport"
      content="width=device-width, initial-scale=1.0">

<title>FriendBook</title>

<style>

* {
    box-sizing: border-box;
}

body {
    margin: 0;
    font-family: Arial, Helvetica, sans-serif;
    background: #f0f2f5;
    color: #1c1e21;
}

.header {
    background: #1877f2;
    color: white;
    padding: 25px;
    text-align: center;
}

.header h1 {
    margin: 0;
    font-size: 38px;
}

.header p {
    margin-top: 8px;
    font-size: 17px;
}

.container {
    width: 90%;
    max-width: 1000px;
    margin: 40px auto;
}

.card {
    background: white;
    border-radius: 12px;
    padding: 30px;
    margin-bottom: 25px;
    box-shadow: 0 3px 12px rgba(0,0,0,0.10);
}

.card h2 {
    margin-top: 0;
    color: #1877f2;
}

input,
select {
    width: 100%;
    padding: 12px;
    margin: 8px 0 15px 0;
    border: 1px solid #ccd0d5;
    border-radius: 7px;
    font-size: 15px;
}

button {
    background: #1877f2;
    color: white;
    border: none;
    padding: 12px 20px;
    border-radius: 7px;
    cursor: pointer;
    font-size: 15px;
}

button:hover {
    background: #145dbf;
}

.feature-grid {
    display: grid;
    grid-template-columns:
        repeat(auto-fit, minmax(220px, 1fr));
    gap: 20px;
}

.feature {
    background: #f7f9fc;
    padding: 20px;
    border-radius: 10px;
    border: 1px solid #e0e3e7;
}

.feature h3 {
    margin-top: 0;
}

.footer {
    text-align: center;
    padding: 25px;
    color: #65676b;
}

</style>

</head>

<body>

<div class="header">

    <h1>FriendBook</h1>

    <p>
        Personalised Friend Recommendation System
    </p>

</div>


<div class="container">

    <div class="card">

        <h2>Welcome to FriendBook 👋</h2>

        <p>
            Connect with people, view profiles and
            discover personalised friend recommendations.
        </p>

    </div>


    <div class="card">

        <h2>Features</h2>

        <div class="feature-grid">

            <div class="feature">

                <h3>👤 User Profiles</h3>

                <p>
                    View your profile and other users'
                    information.
                </p>

            </div>


            <div class="feature">

                <h3>🤝 Friends</h3>

                <p>
                    Manage your friends and connections.
                </p>

            </div>


            <div class="feature">

                <h3>⭐ Recommendations</h3>

                <p>
                    Get personalised friend suggestions
                    based on profile similarity.
                </p>

            </div>


            <div class="feature">

                <h3>💬 Interaction</h3>

                <p>
                    Interact with your friends and improve
                    connection strength.
                </p>

            </div>

        </div>

    </div>


    <div class="card">

        <h2>Login</h2>

        <form action="/login" method="get">

            <label>Username</label>

            <input
                type="text"
                name="username"
                placeholder="Enter username"
                required
            >

            <label>Password</label>

            <input
                type="password"
                name="password"
                placeholder="Enter password"
                required
            >

            <button type="submit">
                Login
            </button>

        </form>

    </div>


    <div class="card">

        <h2>Register New User</h2>

        <form action="/register" method="get">

            <label>Username</label>

            <input
                type="text"
                name="username"
                required
            >


            <label>Password</label>

            <input
                type="password"
                name="password"
                required
            >


            <label>First Name</label>

            <input
                type="text"
                name="firstName"
                required
            >


            <label>Last Name</label>

            <input
                type="text"
                name="lastName"
                required
            >


            <label>College</label>

            <input
                type="text"
                name="college"
                required
            >


            <label>City</label>

            <input
                type="text"
                name="city"
                required
            >


            <label>Age</label>

            <input
                type="number"
                name="age"
                min="1"
                max="120"
                required
            >


            <label>Hobbies</label>

            <input
                type="text"
                name="hobbies"
                placeholder="coding, cricket, gaming"
            >


            <button type="submit">
                Create Account
            </button>

        </form>

    </div>

</div>


<div class="footer">

    FriendBook © 2026

</div>

</body>
</html>
)HTML";

    return html;
}


// ------------------------------------------------------------
// MAIN
// ------------------------------------------------------------

int main()
{
    // Create Social Network object
    SocialNetwork sn;

    // Load existing data
    sn.loadUsers();
    sn.loadFriends();


    // Create HTTP server
    httplib::Server server;


    // --------------------------------------------------------
    // HOME PAGE
    // --------------------------------------------------------

    server.Get(
        "/",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            res.set_content(
                createHomePage(),
                "text/html"
            );
        }
    );


    // --------------------------------------------------------
    // LOGIN
    // --------------------------------------------------------

    server.Get(
        "/login",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            if (!req.has_param("username") ||
                !req.has_param("password"))
            {
                res.set_content(
                    "<h2>Username and password are required.</h2>",
                    "text/html"
                );

                return;
            }


            string username =
                req.get_param_value("username");

            string password =
                req.get_param_value("password");


            int userId =
                sn.getUserIdByUsername(username);


            if (userId == -1)
            {
                res.set_content(
                    R"HTML(
                    <html>
                    <body>
                    <h2>Username not found!</h2>
                    <a href="/">Go Back</a>
                    </body>
                    </html>
                    )HTML",
                    "text/html"
                );

                return;
            }


            User& user =
                sn.getUser(userId);


            if (user.password != password)
            {
                res.set_content(
                    R"HTML(
                    <html>
                    <body>
                    <h2>Incorrect password!</h2>
                    <a href="/">Go Back</a>
                    </body>
                    </html>
                    )HTML",
                    "text/html"
                );

                return;
            }


            string html = R"HTML(
<!DOCTYPE html>
<html>

<head>

<title>FriendBook Dashboard</title>

<style>

body {
    margin: 0;
    font-family: Arial;
    background: #f0f2f5;
}

.header {
    background: #1877f2;
    color: white;
    padding: 20px;
    text-align: center;
}

.container {
    max-width: 900px;
    margin: 30px auto;
}

.card {
    background: white;
    padding: 25px;
    margin-bottom: 20px;
    border-radius: 12px;
    box-shadow: 0 3px 12px rgba(0,0,0,0.1);
}

a {
    display: inline-block;
    background: #1877f2;
    color: white;
    padding: 12px 18px;
    margin: 5px;
    border-radius: 7px;
    text-decoration: none;
}

a:hover {
    background: #145dbf;
}

</style>

</head>

<body>

<div class="header">

<h1>FriendBook</h1>

<p>Welcome to your dashboard</p>

</div>

<div class="container">

<div class="card">

<h2>Welcome, )HTML";

            html += escapeHTML(user.firstName);

            html += " ";

            html += escapeHTML(user.lastName);

            html += R"HTML( 👋</h2>

<p>
You are successfully logged in.
</p>

</div>


<div class="card">

<h2>Profile</h2>

<p>
<b>Username:</b> )HTML";

            html += escapeHTML(user.username);

            html += R"HTML(
</p>

<p>
<b>Name:</b> )HTML";

            html += escapeHTML(user.firstName);

            html += " ";

            html += escapeHTML(user.lastName);

            html += R"HTML(
</p>

<p>
<b>Age:</b> )HTML";

            html += to_string(user.age);

            html += R"HTML(
</p>

<p>
<b>City:</b> )HTML";

            html += escapeHTML(user.locality);

            html += R"HTML(
</p>

</div>


<div class="card">

<h2>FriendBook Features</h2>

<a href="/profile?id=)HTML";

            html += to_string(userId);

            html += R"HTML(">
View Profile
</a>


<a href="/friends?id=)HTML";

            html += to_string(userId);

            html += R"HTML(">
View Friends
</a>


<a href="/suggestions?id=)HTML";

            html += to_string(userId);

            html += R"HTML(">
Friend Suggestions
</a>


<a href="/">
Logout
</a>

</div>

</div>

</body>

</html>
)HTML";

            res.set_content(
                html,
                "text/html"
            );
        }
    );


    // --------------------------------------------------------
    // PROFILE
    // --------------------------------------------------------

    server.Get(
        "/profile",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            if (!req.has_param("id"))
            {
                res.set_content(
                    "<h2>User ID is required.</h2>",
                    "text/html"
                );

                return;
            }


            int userId =
                stoi(req.get_param_value("id"));


            string html = R"HTML(
<html>

<head>

<title>Profile</title>

<style>

body {
    font-family: Arial;
    background: #f0f2f5;
}

.card {
    background: white;
    max-width: 600px;
    margin: 50px auto;
    padding: 30px;
    border-radius: 12px;
    box-shadow: 0 3px 12px rgba(0,0,0,.1);
}

a {
    color: #1877f2;
}

</style>

</head>

<body>

<div class="card">

<h1>Profile</h1>

)HTML";


            try
            {
                User& user =
                    sn.getUser(userId);


                html += "<h2>";

                html += escapeHTML(user.firstName);

                html += " ";

                html += escapeHTML(user.lastName);

                html += "</h2>";


                html += "<p><b>Username:</b> ";

                html += escapeHTML(user.username);

                html += "</p>";


                html += "<p><b>Age:</b> ";

                html += to_string(user.age);

                html += "</p>";


                html += "<p><b>City:</b> ";

                html += escapeHTML(user.locality);

                html += "</p>";


                html += "<p><b>College:</b> ";

                html += escapeHTML(user.schoolName);

                html += "</p>";


                html += "<p><b>Hobbies:</b> ";

                for (size_t i = 0;
                     i < user.hobbies.size();
                     i++)
                {
                    if (i > 0)
                        html += ", ";

                    html +=
                        escapeHTML(user.hobbies[i]);
                }

                html += "</p>";
            }
            catch (...)
            {
                html += "<h2>User not found.</h2>";
            }


            html += R"HTML(

<br>

<a href="/">
Back to Home
</a>

</div>

</body>

</html>

)HTML";


            res.set_content(
                html,
                "text/html"
            );
        }
    );


    // --------------------------------------------------------
    // FRIEND LIST
    // --------------------------------------------------------

    server.Get(
        "/friends",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            if (!req.has_param("id"))
            {
                res.set_content(
                    "<h2>User ID is required.</h2>",
                    "text/html"
                );

                return;
            }


            int userId =
                stoi(req.get_param_value("id"));


            string html = R"HTML(
<html>

<head>

<title>Friends</title>

<style>

body {
    font-family: Arial;
    background: #f0f2f5;
}

.card {
    background: white;
    max-width: 700px;
    margin: 50px auto;
    padding: 30px;
    border-radius: 12px;
}

</style>

</head>

<body>

<div class="card">

<h1>Friends</h1>

)HTML";


            // displayCurrentFriends prints to console,
            // so the web page cannot directly use its output.
            // We provide a basic page instead.

            html += R"HTML(

<p>
Your friend list is stored in the FriendBook
social network.
</p>

<p>
Use the FriendBook desktop application for the
complete friend-list management functionality.
</p>

<br>

<a href="/">
Back to Home
</a>

</div>

</body>

</html>

)HTML";


            res.set_content(
                html,
                "text/html"
            );
        }
    );


    // --------------------------------------------------------
    // FRIEND SUGGESTIONS
    // --------------------------------------------------------

    server.Get(
        "/suggestions",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            if (!req.has_param("id"))
            {
                res.set_content(
                    "<h2>User ID is required.</h2>",
                    "text/html"
                );

                return;
            }


            int userId =
                stoi(req.get_param_value("id"));


            vector<int> suggestions =
                sn.suggestedFriends(userId);


            string html = R"HTML(
<html>

<head>

<title>Friend Suggestions</title>

<style>

body {
    font-family: Arial;
    background: #f0f2f5;
}

.container {
    max-width: 800px;
    margin: 40px auto;
}

.card {
    background: white;
    padding: 20px;
    margin-bottom: 15px;
    border-radius: 10px;
}

h1 {
    color: #1877f2;
}

a {
    color: #1877f2;
}

</style>

</head>

<body>

<div class="container">

<h1>People You May Know 🤝</h1>

)HTML";


            if (suggestions.empty())
            {
                html +=
                    "<div class='card'>"
                    "No friend suggestions available."
                    "</div>";
            }
            else
            {
                for (size_t i = 0;
                     i < suggestions.size();
                     i++)
                {
                    try
                    {
                        User& user =
                            sn.getUser(suggestions[i]);


                        html +=
                            "<div class='card'>";


                        html += "<h2>";

                        html +=
                            escapeHTML(
                                user.firstName
                            );

                        html += " ";

                        html +=
                            escapeHTML(
                                user.lastName
                            );

                        html += "</h2>";


                        html += "<p><b>Username:</b> ";

                        html +=
                            escapeHTML(
                                user.username
                            );

                        html += "</p>";


                        html += "<p><b>City:</b> ";

                        html +=
                            escapeHTML(
                                user.locality
                            );

                        html += "</p>";


                        html +=
                            "<p><b>College:</b> ";

                        html +=
                            escapeHTML(
                                user.schoolName
                            );

                        html += "</p>";


                        html +=
                            "<a href='/profile?id=";

                        html +=
                            to_string(user.id);

                        html +=
                            "'>View Profile</a>";


                        html +=
                            "</div>";
                    }
                    catch (...)
                    {
                        // Ignore invalid users
                    }
                }
            }


            html += R"HTML(

<br>

<a href="/">
Back to Home
</a>

</div>

</body>

</html>

)HTML";


            res.set_content(
                html,
                "text/html"
            );
        }
    );


    // --------------------------------------------------------
    // REGISTER USER
    // --------------------------------------------------------

    server.Get(
        "/register",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            if (!req.has_param("username") ||
                !req.has_param("password") ||
                !req.has_param("firstName") ||
                !req.has_param("lastName") ||
                !req.has_param("college") ||
                !req.has_param("city") ||
                !req.has_param("age"))
            {
                res.set_content(
                    "<h2>Please fill all required fields.</h2>"
                    "<a href='/'>Go Back</a>",
                    "text/html"
                );

                return;
            }


            string username =
                req.get_param_value("username");


            // Check existing username
            if (sn.getUserIdByUsername(username) != -1)
            {
                res.set_content(
                    "<h2>Username already exists.</h2>"
                    "<a href='/'>Go Back</a>",
                    "text/html"
                );

                return;
            }


            string password =
                req.get_param_value("password");

            string firstName =
                req.get_param_value("firstName");

            string lastName =
                req.get_param_value("lastName");

            string college =
                req.get_param_value("college");

            string city =
                req.get_param_value("city");

            int age =
                stoi(req.get_param_value("age"));


            vector<string> hobbies;


            if (req.has_param("hobbies"))
            {
                string hobbyInput =
                    req.get_param_value("hobbies");


                stringstream ss(hobbyInput);

                string hobby;


                while (
                    getline(ss, hobby, ',')
                )
                {
                    // Remove leading spaces
                    hobby.erase(
                        hobby.begin(),
                        find_if(
                            hobby.begin(),
                            hobby.end(),
                            [](unsigned char ch)
                            {
                                return !isspace(ch);
                            }
                        )
                    );


                    // Remove trailing spaces
                    hobby.erase(
                        find_if(
                            hobby.rbegin(),
                            hobby.rend(),
                            [](unsigned char ch)
                            {
                                return !isspace(ch);
                            }
                        ).base(),
                        hobby.end()
                    );


                    if (!hobby.empty())
                    {
                        hobbies.push_back(hobby);
                    }
                }
            }


            int newId =
                sn.getNextUserId();


            User newUser(
                newId,
                username,
                firstName,
                lastName,
                college,
                city,
                age,
                hobbies
            );


            newUser.password =
                password;


            sn.registerUser(newUser);

            sn.saveUsers();


            string html = R"HTML(
<html>

<head>

<title>Registration Successful</title>

<style>

body {
    font-family: Arial;
    background: #f0f2f5;
}

.card {
    background: white;
    max-width: 600px;
    margin: 80px auto;
    padding: 40px;
    text-align: center;
    border-radius: 12px;
}

h1 {
    color: #1877f2;
}

a {
    display: inline-block;
    background: #1877f2;
    color: white;
    padding: 12px 20px;
    border-radius: 7px;
    text-decoration: none;
}

</style>

</head>

<body>

<div class="card">

<h1>Registration Successful 🎉</h1>

<p>Your FriendBook account has been created.</p>

<p>You can now login using your username and password.</p>

<a href="/">
Go to Login
</a>

</div>

</body>

</html>
)HTML";


            res.set_content(
                html,
                "text/html"
            );
        }
    );


    // ------------------------------------------------------------
    // START SERVER
    // ------------------------------------------------------------

    cout << endl;
    cout << "========================================" << endl;
    cout << "          FRIENDBOOK WEB SERVER" << endl;
    cout << "========================================" << endl;

    cout << "Users loaded successfully." << endl;

    cout << endl;

    // Render provides the PORT environment variable.
    // For local testing, if PORT is not set, use 8080.
    const char* portEnv = getenv("PORT");

    int port = 8080;

    if (portEnv != nullptr)
    {
        try
        {
            port = stoi(portEnv);
        }
        catch (...)
        {
            port = 8080;
        }
    }

    cout << "Server running on port: "
         << port << endl;

    cout << "Listening on: 0.0.0.0"
         << endl;

    cout << endl;

    cout << "Press CTRL+C to stop the server." << endl;

    cout << "========================================" << endl;

    // 0.0.0.0 allows Render and local network access.
    if (!server.listen("0.0.0.0", port))
    {
        cerr << "Failed to start server." << endl;

        return 1;
    }

    return 0;
}