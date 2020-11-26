from flask import jsonify
from app import app, app_logger
from app.db import db
from app.models.ship import Ship

logger = app_logger.get_logger(__name__)

@app.route('/get_ships', methods=["GET"])
def get_ships():
    try:
        logger.info('Handling request: Getting a list of records')
        query_ship_list = db.session.query(Ship)
        response = jsonify([i.serialize for i in query_ship_list.all()])
        return response
    except Exception as e:
        logger.error(str(e))