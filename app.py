from flask import Flask, request, render_template
import datetime
import sqlite3
import sqlite3 as sql
def createdb():

	conn = sqlite3.connect('database.db')
	# print("Opened database successfully")
	conn.execute('CREATE TABLE IF NOT EXISTS clients (id TEXT , latitude REAL, longitude REAL, severity NUMBER, relative NUMBER, msg BOOL)')
	# print("Table Created successfully");

app = Flask(__name__)

@app.route('/')
def abc():
	createdb();	
	return render_template('index.html'	)

@app.route('/addrec', methods =['POST'])	
def addentryindb():
	createdb();
	idx = request.form.get('id',default = "abcd@connectaide",type = str)
	longitude = request.form.get('latitude',default= 0.0, type = float)
	latitude = request.form.get('longitude', default=0.0,type=float)
	severity = request.form.get('severity', default = 0, type = int)
	relative = request.form.get('relative', default = 1234567890,type = int)
	msg = request.form.get('msg', defaul t= False, type = bool)
	try:

		with sql.connect('database.db') as con:
			cur= con.cursor()
			cur.execute('INSERT INTO clients (id,latitude,longitude,severity,relative,msg) VALUES (?,?,?,?,?,?)',(idx,latitude,longitude,severity,relative,msg))
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
	app.run(host='0.0.0.0',debug=True)
