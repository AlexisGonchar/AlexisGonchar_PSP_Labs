from app import app_logger
import pathlib
import yaml

logger = app_logger.get_logger(__name__)


BASE_DIR = pathlib.Path(__file__).parent.parent
DEFAULT_CONFIG_PATH = str(BASE_DIR / 'config' / 'config.yaml')


def get_config():
    try:
        with open(DEFAULT_CONFIG_PATH) as f:
            data = yaml.load(f, Loader=yaml.FullLoader)
            return data
    except Exception as e:
        logger.error(str(e))


def get_db_config():
    return get_config()['postgres']