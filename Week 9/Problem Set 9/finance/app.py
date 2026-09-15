import os
import sqlite3

from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)
app.config["SECRET_KEY"] = os.urandom(32)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

def get_db():
    """Create a SQLite connection bound to the current request thread."""
    conn = sqlite3.connect("finance.db")
    conn.row_factory = sqlite3.Row
    return conn


def initialize_db():
    """Create required tables if they do not already exist."""
    with get_db() as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
                username TEXT NOT NULL,
                hash TEXT NOT NULL,
                cash NUMERIC NOT NULL DEFAULT 10000.00
            )
            """
        )
        conn.execute(
            """
            CREATE UNIQUE INDEX IF NOT EXISTS username_idx ON users (username)
            """
        )
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS transactions (
                id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
                user_id INTEGER NOT NULL,
                symbol TEXT NOT NULL,
                shares INTEGER NOT NULL,
                price NUMERIC NOT NULL,
                transacted TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
                type TEXT NOT NULL CHECK(type IN ('BUY', 'SELL'))
            )
            """
        )


initialize_db()


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks."""
    with get_db() as conn:
        user = conn.execute(
            "SELECT cash FROM users WHERE id = ?", (session["user_id"],)
        ).fetchone()
        cash = float(user["cash"]) if user else 0.0

        positions = conn.execute(
            """
            SELECT symbol,
                   SUM(CASE WHEN type = 'BUY' THEN shares ELSE -shares END) AS total_shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            HAVING total_shares > 0
            ORDER BY symbol
            """,
            (session["user_id"],),
        ).fetchall()

    rows = []
    total_value = cash

    for position in positions:
        quote = lookup(position["symbol"])
        if quote is None:
            continue
        shares = int(position["total_shares"])
        price = float(quote["price"])
        value = shares * price
        rows.append(
            {
                "symbol": quote["symbol"],
                "name": quote["name"],
                "shares": shares,
                "price": price,
                "total": value,
            }
        )
        total_value += value

    return render_template("index.html", rows=rows, cash=cash, total=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        symbol = request.form.get("symbol", "").strip()
        shares = request.form.get("shares", "").strip()

        if not symbol:
            return apology("must provide symbol", 400)
        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("shares must be a positive integer", 400)

        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol", 400)

        shares = int(shares)
        total_cost = quote["price"] * shares

        with get_db() as conn:
            user = conn.execute(
                "SELECT cash FROM users WHERE id = ?", (session["user_id"],)
            ).fetchone()

            if user is None or float(user["cash"]) < total_cost:
                return apology("insufficient funds", 400)

            conn.execute(
                "INSERT INTO transactions (user_id, symbol, shares, price, type) VALUES (?, ?, ?, ?, 'BUY')",
                (session["user_id"], quote["symbol"], shares, quote["price"]),
            )
            conn.execute(
                "UPDATE users SET cash = cash - ? WHERE id = ?",
                (total_cost, session["user_id"]),
            )

        flash("Bought shares.")
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    with get_db() as conn:
        transactions = conn.execute(
            """
            SELECT symbol, shares, price, transacted, type
            FROM transactions
            WHERE user_id = ?
            ORDER BY id DESC
            """,
            (session["user_id"],),
        ).fetchall()
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    session.clear()

    if request.method == "POST":
        username = request.form.get("username", "").strip()
        password = request.form.get("password", "")

        if not username:
            return apology("must provide username", 403)
        if not password:
            return apology("must provide password", 403)

        with get_db() as conn:
            row = conn.execute(
                "SELECT * FROM users WHERE username = ?", (username,)
            ).fetchone()

        if row is None or not check_password_hash(row["hash"], password):
            return apology("invalid username and/or password", 403)

        session["user_id"] = row["id"]
        return redirect("/")

    return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""
    session.clear()
    return redirect("/login")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol", "").strip()
        if not symbol:
            return apology("must provide symbol", 400)

        quote_data = lookup(symbol)
        if quote_data is None:
            return apology("invalid symbol", 400)

        return render_template("quote.html", quote=quote_data)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        username = request.form.get("username", "").strip()
        password = request.form.get("password", "")
        confirmation = request.form.get("confirmation", "")

        if not username:
            return apology("must provide username", 400)
        if not password:
            return apology("must provide password", 400)
        if password != confirmation:
            return apology("passwords do not match", 400)

        with get_db() as conn:
            existing = conn.execute(
                "SELECT id FROM users WHERE username = ?", (username,)
            ).fetchone()
            if existing is not None:
                return apology("username already exists", 400)

            conn.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)",
                (username, generate_password_hash(password)),
            )
            user = conn.execute(
                "SELECT id FROM users WHERE username = ?", (username,)
            ).fetchone()

        session["user_id"] = user["id"]
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        symbol = request.form.get("symbol", "").strip().upper()
        shares = request.form.get("shares", "").strip()

        if not symbol:
            return apology("must provide symbol", 400)
        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("shares must be a positive integer", 400)

        shares = int(shares)

        with get_db() as conn:
            owned = conn.execute(
                """
                SELECT COALESCE(SUM(CASE WHEN type = 'BUY' THEN shares ELSE -shares END), 0) AS total_shares
                FROM transactions
                WHERE user_id = ? AND symbol = ?
                """,
                (session["user_id"], symbol),
            ).fetchone()

            if owned is None or int(owned["total_shares"]) < shares:
                return apology("too many shares", 400)

            quote = lookup(symbol)
            if quote is None:
                return apology("invalid symbol", 400)

            sale_total = quote["price"] * shares
            conn.execute(
                "INSERT INTO transactions (user_id, symbol, shares, price, type) VALUES (?, ?, ?, ?, 'SELL')",
                (session["user_id"], quote["symbol"], shares, quote["price"]),
            )
            conn.execute(
                "UPDATE users SET cash = cash + ? WHERE id = ?",
                (sale_total, session["user_id"]),
            )

        flash("Sold shares.")
        return redirect("/")

    with get_db() as conn:
        positions = conn.execute(
            """
            SELECT symbol,
                   SUM(CASE WHEN type = 'BUY' THEN shares ELSE -shares END) AS total_shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            HAVING total_shares > 0
            ORDER BY symbol
            """,
            (session["user_id"],),
        ).fetchall()
    return render_template("sell.html", positions=positions)
