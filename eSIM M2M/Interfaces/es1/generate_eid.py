import random
import requests

def generate_32_digit_number():
    # Ensure the number starts with 89091032
    start = '89091032'
    # Generate the remaining 24 digits randomly
    while True:
        remaining_digits = ''.join([str(random.randint(0, 9)) for _ in range(24)])
        EID = start + remaining_digits
        if int(EID) % 97 == 1:
            break
    return EID

def send_to_sm_sr(EID):
    url = 'http://localhost:5000/store_eid'
    data = {'EID': EID}
    response = requests.post(url, json=data)
    return response.json()

if __name__ == "__main__":
    eid = generate_32_digit_number()
    response = send_to_sm_sr(eid)
    print(response)

