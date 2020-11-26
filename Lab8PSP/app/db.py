from flask_sqlalchemy import SQLAlchemy
from app import app, settings, app_logger


logger = app_logger.get_logger(__name__)

data = settings.get_db_config()

user = data['user']
password = data['password']
host = data['host']
database = data['database']


try:
    app.config['SQLALCHEMY_DATABASE_URI'] = f'postgresql://{user}:{password}@{host}/{database}'
    db = SQLAlchemy(app)
except Exception as e:
    logger.error(str(e))