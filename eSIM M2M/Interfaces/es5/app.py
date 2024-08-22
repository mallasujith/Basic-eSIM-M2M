from flask import Flask, jsonify, render_template
import requests

app = Flask(__name__)

# URL for es1 endpoint
ES2_URL = 'http://localhost:5000'

# Function to fetch EID information from es1
def fetch_eid_info_from_es1():
    try:
        response = requests.get(f"{ES2_URL}/get_eids")
        if response.status_code == 200:
            return response.json()
        else:
            return {'error': 'Failed to fetch EID information from es1'}
    except requests.exceptions.RequestException as e:
        return {'error': f'Request to es1 failed: {str(e)}'}

@app.route('/')
def index():
    # Fetch data from es1
    eid_info = fetch_eid_info_from_es1()
    
    return render_template('index.html', eid_info=eid_info)

if __name__ == "__main__":
    app.run(port=5005, debug=True)

