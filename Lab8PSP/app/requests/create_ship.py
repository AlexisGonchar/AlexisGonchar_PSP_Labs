from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.ship import Ship

logger = app_logger.get_logger(__name__)


@app.route('/create_ship', methods=["POST"])
def create_ship():
    response = None
    try:
        logger.info('Handling request: Adding a record to a table')
        ship_name = request.args.get('name')
        ship_type = request.args.get('type')
        ship_displacement = request.args.get('displacement')
        ship_country = request.args.get('country')
        ship = Ship(name=ship_name, type=ship_type,
                    displacement=ship_displacement, country=ship_country)
        ship.validate()
        db.session.add(ship)
        db.session.commit()
        response = jsonify({'success': 'Record added successfully.'})
        response.status_code = 200
    except Exception as e:
        db.session.rollback()
        response = jsonify({'error': 'The request had invalid values.'})
        response.status_code = 400
        logger.error(str(e))
    finally:
        return response
