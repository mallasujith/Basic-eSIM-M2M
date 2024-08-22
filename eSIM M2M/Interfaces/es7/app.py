from flask import Flask, render_template, request

app = Flask(__name__)

# Home route to display options
@app.route('/')
def index():
    return render_template('index.html')

# Route to handle option selections
@app.route('/option', methods=['POST'])
def handle_option():
    option = request.form['option']
    message = ''

    if option == 'enable':
        message = 'esim profile is enabled'
    elif option == 'disable':
        message = 'esim profile is disabled'
    elif option == 'delete':
        message = 'esim profile is deleted'
    elif option == 'switch':
        message = 'esim profile is switched'
    else:
        message = 'Unknown option selected'

    return render_template('index.html', message=message)

if __name__ == "__main__":
    app.run(port=5006, debug=True)

