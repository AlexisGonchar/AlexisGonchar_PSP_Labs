# Task

##Flask project

###How to start this project:

1. Open project root directory in terminal.

2. Create a virtual environment for the project: 
   
####   $ virtualenv venv

3. Activate virtual environment:
 
####   $ source venv/bin/activate

4. Install packages:

#### $ pip install -r requirements.txt

5. Create PostgreSQL database. Run init_models.py for creating tables in database:

####   $ python3 init_models.py

6. Change the configuration file in the directory config.

7. Start the server:
   
####   $ python3 manage.py runserver -d -r --host=127.0.0.1 --port 8000

    