from flask import Flask, request, jsonify, render_template
from pymongo import MongoClient

app = Flask(__name__)

# Connect to MongoDB
client = MongoClient('localhost', 27017)
db = client['sm_sr_db']
collection = db['eis']

# Clear the collection before starting
# collection.delete_many({})

@app.route('/store_eid', methods=['POST'])
def store_eid():
    data = request.json
    EID = data.get('EID')
    
    if not EID:
        return jsonify({'error': 'No EID provided'}), 400
    
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

@app.route('/get_eids', methods=['GET'])
def get_eids():
    eids = list(collection.find({}, {'_id': 0}))
    return jsonify(eids)

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == "__main__":
    app.run(debug=True)

