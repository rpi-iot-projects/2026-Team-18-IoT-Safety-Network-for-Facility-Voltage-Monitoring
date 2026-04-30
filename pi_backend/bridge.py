import paho.mqtt.client as mqtt
import sqlite3
import json

DB_PATH = "fwncvsd.db"

def init_db():
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()
    cursor.execute('''CREATE TABLE IF NOT EXISTS telemetry 
                     (id INTEGER PRIMARY KEY AUTOINCREMENT, node TEXT, voltage REAL, 
                      timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)''')
    conn.commit()
    conn.close()

def on_message(client, userdata, message):
    try:
        data = json.loads(message.payload.decode())
        conn = sqlite3.connect(DB_PATH)
        cursor = conn.cursor()
        cursor.execute("INSERT INTO telemetry (node, voltage) VALUES (?, ?)", (data['node'], data['voltage']))
        # Maintain circular buffer
        cursor.execute("DELETE FROM telemetry WHERE id NOT IN (SELECT id FROM telemetry ORDER BY id DESC LIMIT 500)")
        conn.commit()
        conn.close()
    except Exception as e: print(f"DB Error: {e}")

init_db()
client = mqtt.Client("Bridge_Service")
client.on_message = on_message
client.connect("localhost", 1883)
client.subscribe("fwncvsd/telemetry/live")
client.loop_forever()
