from flask import Flask, request, jsonify, render_template
from pymongo import MongoClient
import re

app = Flask(__name__)

# Connect to MongoDB
client = MongoClient('localhost', 27017)
db = client['sm_sr_db']
collection = db['eis']

# Clear the collection before starting
collection.delete_many({})

# Validation functions
def validate_eid(eid):
    return bool(re.match(r'^\d{32}$', eid))

@app.route('/store_eid', methods=['POST'])
def store_eid():
    data = request.json
    EID = data.get('EID')
    
    if not EID:
        return jsonify({'error': 'EID is required'}), 400
    
    if not validate_eid(EID):
        return jsonify({'error': 'Invalid EID format'}), 400
    
    ICCID = "89918540100015042627"  # Predefined ICCID
    IMEI = "355123375840156"  # Predefined IMEI
    
    register_number = collection.count_documents({}) + 1
    
    industry_identifier = EID[0]
    specifying_telecommunication = EID[1]
    country_code = EID[2:5]
    issuer_identifier = EID[5:8]
    platform_version = EID[8:13]
    os_version = EID[13:18]
    individual_identification_number = EID[18:30]
    check_digits = EID[30:32]
    
    eid_entry = {
        'register_number': register_number,
        'ICCID': ICCID,
        'IMEI': IMEI,
        'EIS_information': {
            'Industry Identifier': industry_identifier,
            'Specifying Telecommunication': specifying_telecommunication,
            'Country Code': country_code,
            'Issuer Identifier': issuer_identifier,
            'Platform Version': platform_version,
            'OS Version': os_version,
            'Individual Identification Number': individual_identification_number,
            'Check Digits': check_digits
        },
        'EID': EID
    }
    
    collection.insert_one(eid_entry)
    
    return jsonify({'message': 'EID stored successfully', 'register_number': register_number})

@app.route('/get_eid_info', methods=['POST'])
def get_eid_info():
    eid_number = request.form.get('eid_number')
    
    if not validate_eid(eid_number):
        return jsonify({'error': 'Invalid EID format'}), 400
    
    eid_entry = collection.find_one({'EID': eid_number}, {'_id': 0})
    
    if not eid_entry:
        return jsonify({'error': 'EID not found'}), 404
    
    return jsonify(eid_entry)

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == "__main__":
    app.run(port=5002, debug=True)
