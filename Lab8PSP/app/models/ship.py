from app.db import db


class Ship(db.Model):
    __tablename__ = 'ship'
    id=db.Column('id', db.Integer, primary_key=True)
    name=db.Column('name', db.String(32))
    type=db.Column('type', db.String(32))
    displacement=db.Column('displacement', db.Float)
    country = db.Column('country', db.String(32))

    def validate(self):
        try:
            float(self.displacement)
        except:
            raise Exception('Invalid data.')

    @property
    def serialize(self):
        return {'ship': {'id':  self.id, 'name': self.name, 'type': self.type,
                         'displacement': self.displacement, 'country': self.country }}

    @property
    def serialize_many2many(self):
        return [item.serialize for item in self.many2many]


