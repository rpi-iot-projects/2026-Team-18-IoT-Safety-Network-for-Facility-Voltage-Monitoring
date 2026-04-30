from flask import Flask, render_template, jsonify, request
import sqlite3

app = Flask(__name__, template_folder='../webapp', static_folder='../webapp')
DB_PATH = "fwncvsd.db"

@app.route('/')
def index(): return render_template('index.html')

@app.route('/api/data')
def get_data():
    node_filter = request.args.get('node')
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    cursor = conn.cursor()
    if node_filter and node_filter != 'ALL':
        cursor.execute("SELECT node, voltage, datetime(timestamp, 'localtime') as timestamp FROM telemetry WHERE node = ? ORDER BY id DESC LIMIT 20", (node_filter,))
    else:
        cursor.execute("SELECT node, voltage, datetime(timestamp, 'localtime') as timestamp FROM telemetry ORDER BY id DESC LIMIT 20")
    rows = cursor.fetchall()
    conn.close()
    return jsonify([dict(row) for row in rows])

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
