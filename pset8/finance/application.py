import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response
    

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get the cash and other relevant info for the user
    cash_tot = 0.00
    cash = db.execute("SELECT cash FROM users where id = :userid;", userid=session["user_id"])[-1]["cash"]
    row = db.execute("SELECT symbol, name, SUM(shares), price, cash FROM buy WHERE id = :userid GROUP BY symbol;",
                     userid=session["user_id"])

    # Calculate the total cash
    for item in row:
        cash_tot += item["price"] * item["SUM(shares)"]

    return render_template("index.html", length=len(row), row=row, cash=("{:,.2f}".format(cash)), cash_tot=("{:,.2f}".format(cash_tot+cash)))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        # Ensure symbol and shares was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)
        if not request.form.get("shares"):
            return apology("must provide number of shares", 403)

        # Ensure symbol exists
        symbol = lookup(request.form.get("symbol"))
        if not symbol:
            return apology("symbol invalid", 403)

        # Ensure number of shares can be bought
        user_data = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session["user_id"])
        cash_left = user_data[-1]["cash"] - float(request.form.get("shares"))*symbol["price"]

        if cash_left < 0:
            return apology("do not have enough cash", 403)
        else:
            db.execute("INSERT INTO buy (shares, price, cash, transacted, name, symbol, id) VALUES (:shares, :price, :cash, CURRENT_TIMESTAMP, :name, :symbol, :userid);",
                       userid=session["user_id"], shares=request.form.get("shares"), price=symbol["price"], cash=cash_left, name=symbol["name"], symbol=symbol["symbol"])
            db.execute("UPDATE users SET cash = :cash WHERE id = :userid;", cash=cash_left, userid=session["user_id"])
            db.execute("INSERT INTO history (id, traded, price, symbol, transacted) VALUES (:userid, :traded, :price, :symbol, CURRENT_TIMESTAMP);",
                       userid=session["user_id"], traded=int(request.form.get("shares")), price=symbol["price"], symbol=symbol["symbol"])

            # Flash a message and go to homepage
            flash("Bought!")
            return redirect("/")

    # User reached route via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get all necessary information from table
    data = db.execute("SELECT symbol, traded, price, transacted FROM history WHERE id = :userid GROUP BY transacted;", 
                      userid=session["user_id"])

    return render_template("history.html", data=data, length=len(data))


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST
    if request.method == "POST":

        # Get the symbol
        symbol = request.form.get("symbol")
        dictQuote = lookup(symbol)

        # Lookup the symbol and render a second html page for the result
        if dictQuote is not None:
            data = "A share of " + dictQuote["name"] + "(" + dictQuote["symbol"] + ") costs $" + str(dictQuote["price"])
            return render_template("quoted.html", data=data)
        else:
            return apology("invalid symbol", 403)

    # User reached route via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was typed
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was typed
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password was again typed for confirmation
        elif not request.form.get("confirmation"):
            return apology("must confirm password too", 403)

        # Check if username exits
        row = db.execute("SELECT username FROM users WHERE username = :username;",
                         username=request.form.get("username"))
        if row:
            return apology("username already exists", 403)

        # Check if passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 403)

        # Insert into the table
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashs);",
                   username=request.form.get("username"), hashs=generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    data = db.execute("SELECT shares, symbol FROM buy WHERE id = :userid GROUP BY symbol;", userid=session["user_id"])
    # User reached route via POST
    if request.method == "POST":

        # Check if shares was submitted
        if not request.form.get("shares"):
            return apology("please enter a number to sell", 403)

        # Check if symbol was submitted
        if not request.form.get("symbol"):
            return apology("please select a share to sell", 403)

        # Check if share doesn't exceed the current hold
        sym = db.execute("SELECT shares FROM buy WHERE id = :userid AND symbol = :symbol", 
                         userid=session["user_id"], symbol=request.form.get("symbol"))[0]['shares']
        if int(request.form.get("shares")) > sym:
            return apology("do not have enough shares", 403)

        # Sell and update the tables
        symbol = lookup(request.form.get("symbol"))
        user_data = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session["user_id"])
        cash_got = user_data[-1]["cash"] + float(request.form.get("shares"))*symbol["price"]
        shares_left = sym - int(request.form.get("shares"))

        db.execute("UPDATE buy SET shares = :shares, price = :price, cash = :cash, transacted = CURRENT_TIMESTAMP WHERE symbol = :symbol AND id = :userid;",
                   userid=session["user_id"], shares=shares_left, price=symbol["price"], cash=cash_got, symbol=symbol["symbol"])
        db.execute("UPDATE users SET cash = :cash WHERE id = :userid;", cash=cash_got, userid=session["user_id"])
        db.execute("INSERT INTO history (id, traded, price, symbol, transacted) VALUES (:userid, :traded, :price, :symbol, CURRENT_TIMESTAMP);",
                   userid=session["user_id"], traded=-int(request.form.get("shares")), price=symbol["price"], symbol=symbol["symbol"])

        # Delete rows if shares go to zero
        if shares_left == 0:
            db.execute("DELETE FROM buy WHERE shares = 0;")

        # Flash a message and return to homepage
        flash("Sold!")
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("sell.html", data=data, length=len(data))


@app.route("/changepass", methods=["GET", "POST"])
@login_required
def changepass():
    """Change the current password"""

    # User reached via POST
    if request.method == "POST":
        # Ensure username was typed
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure original password was typed
        elif not request.form.get("original"):
            return apology("must provide original password", 403)

        # Ensure password was typed
        elif not request.form.get("password"):
            return apology("must provide new password", 403)

        # Ensure password was again typed for confirmation
        elif not request.form.get("confirmation"):
            return apology("must confirm password too", 403)

        # Check if username exits
        row = db.execute("SELECT username, hash FROM users WHERE username = :username;",
                         username=request.form.get("username"))
        if not row:
            return apology("username doesn't exists", 403)

        # Check if original passwords match
        if not check_password_hash(row[0]["hash"], request.form.get("original")):
            return apology("original password do not match", 403)

        # Check if new passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 403)

        # Update the table
        newhash = generate_password_hash(request.form.get("password"))
        db.execute("UPDATE users SET hash = :hashs WHERE id = :userid;", hashs=newhash, userid=session["user_id"])

        # Redirect user to home page
        flash("Password changed!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("changepass.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
