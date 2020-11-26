from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.ship import Ship

logger = app_logger.get_logger(__name__)


@app.route('/update_ship', methods=["POST"])
def update_ship():
    response = None
    try:
        logger.info('Handling request: Updating a record in a table')
        ship_id = request.args.get('id')
        ship_name = request.args.get('name')
        ship_type = request.args.get('type')
        ship_displacement = request.args.get('displacement')
        ship_country = request.args.get('country')
        ship = db.session.query(Ship).get(ship_id)
        ship.name = ship_name
        ship.type = ship_type
        ship.displacement = ship_displacement
        ship.country = ship_country
        ship.validate()
        db.session.commit()
        response = jsonify({'success': 'Record successfully updated.'})
        response.status_code = 200
    except Exception as e:
        db.session.rollback()
        response = jsonify({'error': 'The request had invalid values.'})
        response.status_code = 400
        logger.error(str(e))
    finally:
        return response
