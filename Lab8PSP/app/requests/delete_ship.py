from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.ship import Ship

logger = app_logger.get_logger(__name__)


@app.route('/delete_ship', methods=["POST"])
def delete_ship():
    response = None
    try:
        logger.info('Handling request: Delete record by id')
        ship_id = request.args.get('id')
        db.session.query(Ship).filter_by(id=ship_id).delete()
        db.session.commit()
        response = jsonify({'success': 'Record successfully deleted.'})
        response.status_code = 200
    except Exception as e:
        db.session.rollback()
        response = jsonify({'error': 'The request had invalid values.'})
        response.status_code = 400
        logger.error(str(e))
    finally:
        return response