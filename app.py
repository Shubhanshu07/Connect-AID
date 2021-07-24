from flask import Flask, request, render_template
import datetime
import sqlite3
import sqlite3 as sql
def createdb():

	conn = sqlite3.connect('database.db')
	print("Opened database successfully")
	conn.execute('CREATE TABLE IF NOT EXISTS clients (Phonenumber Number, Name TEXT, location TEXT, severity TEXT, contact_list NUMBER)')
	print("Table Created successfully");

app = Flask(__name__)

@app.route('/')
def abc():
	createdb();	
	return render_template('index.html'	)

@app.route('/addrec', methods =['POST'])	
def addentryindb():
	phone_number = request.args.get('Phonenumber',default= 1221, type = int)
	name = request.args.get('Name', type = str)
	location = request.args.get('location', default=0.0,type=float)
	severity = request.args.get('severity', default = 1, type =int)
	contact = request.args.get('contact', default = 1331,type = int)
	try:

		with sql.connect('database.db') as con:
			cur= con.cursor()
			cur.execute('INSERT INTO clients (phonenumber,Name,location,severity,contact_list) VALUES (?,?,?,?,?)',(phone_number,name,location,severity,contact))
			con.commit()
			msg = 'Record Added successfully'
			print(msg)

	except:
		con.rollback()
		msg = "error in insert operation"
	
	finally:
		return msg
		con.close()


if __name__ == '__main__':
	app.run(debug=True)
