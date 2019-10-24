#!/usr/bin/env python3

from flask import Flask, render_template, json, request, redirect

monitoring = []
app = Flask(__name__)

@app.route("/")
def main():
	kwargs = {"monitoring": monitoring, "count": len(monitoring)}

	return render_template("wlm.html", **kwargs)

@app.route("/water_level_monitoring")
def water_monitoring():

	monitoring.append((request.args.get("id1"), request.args.get("id2"), request.args.get("id4"), request.args.get("id8")))

	f = open("example_config_file")
	config = f.read()
	f.close()

	return config

if __name__ == "__main__":
	app.run(host='0.0.0.0')