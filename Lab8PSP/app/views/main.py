from flask import render_template
from app import app, app_logger


logger = app_logger.get_logger(__name__)


@app.route('/')
@app.route('/index')
def index():
    return render_template("html/index.html")
