#include "rectangularcoordinatesystem.h"
#include <QDataStream>
#include <QVector3D>
#include "cylindercoordinatesystem.h"

namespace CGL {

RectangularCoordinateSystem::RectangularCoordinateSystem(const QVector3D& d, const QVector3D& z, const QVector3D& p)
    : a(d)
    , oz(z - d)
    , oy(QVector3D::crossProduct(oz,p - d))
    , ox(QVector3D::crossProduct(oy,oz))
{
    oz.normalize();
    oy.normalize();
    ox.normalize();
}

RectangularCoordinateSystem::RectangularCoordinateSystem() {}

void RectangularCoordinateSystem::toGlobal(QVector3D& v) const
{
    QVector3D n(v);
    v.setX(ox.x() * n.x() + oy.x() * n.y() + oz.x() * n.z());
    v.setY(ox.y() * n.x() + oy.y() * n.y() + oz.y() * n.z());
    v.setZ(ox.z() * n.x() + oy.z() * n.y() + oz.z() * n.z());

    v += a;
}

void RectangularCoordinateSystem::save(QDataStream& to) const
{
    to << type() << a << ox << oy << oz;
}


RectangularCoordinateSystem* RectangularCoordinateSystem::load(QDataStream& in)
{
    int typ;
    in >> typ;
    RectangularCoordinateSystem* g;
    switch (typ) {
    case Rectangular:
        g = new RectangularCoordinateSystem;
        break;
    case Cylinder:
        g = new CylinderCoordinateSystem;
        break;
    }
    in >> g->a >> g->ox >> g->oy >> g->oz;
    return g;
}


}
