import paho.mqtt.client as mqtt

def on_message(client, userdata, msg):
    print(f"[BMS] SoC Update: {msg.payload.decode()} %")

client = mqtt.Client()
client.on_message = on_message
client.connect("localhost", 1883, 60)
client.subscribe("bms/soc")
client.loop_forever()
