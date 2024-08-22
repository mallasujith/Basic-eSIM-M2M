from flask import Flask, request, jsonify, render_template

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html', message=None)

@app.route('/send_request', methods=['POST'])
def send_request():
    # Simulate sending request to sm-sr (es1)
    installation_message = "Installation request obtained from sm-dp for installation of isd-p on euicc"
    return render_template('index.html', message=installation_message)

if __name__ == "__main__":
    app.run(port=5004, debug=True)

