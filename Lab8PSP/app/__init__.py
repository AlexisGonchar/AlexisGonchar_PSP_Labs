from flask import Flask

app = Flask(__name__)

from app.views import main

from app.requests import get_ships
from app.requests import delete_ship
from app.requests import create_ship
from app.requests import update_ship
