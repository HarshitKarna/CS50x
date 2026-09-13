import sqlite3
from flask import Flask, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure birthdays.db using sqlite3; creates new if doesnt exist
def get_db():
    db = sqlite3.connect("birthdays.db")
    db.row_factory = sqlite3.Row
    return db


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    birthdays_conn = get_db()
    birthdays_cursor = birthdays_conn.cursor()
    if request.method == "POST":

        # Add the user's entry into the database
        person_name = request.form.get("name")
        bday_month = int(request.form.get("month"))
        bday_day = int(request.form.get("day"))
        
        if bday_month <= 0 or bday_month > 12:
            return render_template("error.html", message="Invalid Month")
        
        if bday_day <= 0 or bday_day > 31:
            return render_template("error.html", message="Invalid Day")
        
        birthdays_cursor.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", (person_name, bday_month, bday_day))
        
        birthdays_conn.commit()
        birthdays_conn.close()

        return redirect("/")

    else:

        # Display the entries in the database on index.html
        birthdays_cursor.execute("SELECT * FROM birthdays")
        people = birthdays_cursor.fetchall()
        
        birthdays_conn.close()

        return render_template("index.html", people=people)


@app.route("/delete", methods = ["POST"])
def delete():
    birthdays_conn = get_db()
    birthdays_cursor = birthdays_conn.cursor()

    person_id = request.form.get("id")

    birthdays_cursor.execute("DELETE FROM birthdays WHERE id = ?", (person_id,))

    birthdays_conn.commit()
    birthdays_conn.close()

    return redirect("/")
