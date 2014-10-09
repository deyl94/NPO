#include "cvertexes.h"
#include <QVector3D>
#include "cgl.h"
#include <cmath>
#include <QDataStream>
namespace CGL {




CVertexes::CVertexes(int size, float val) : std::vector<float>(size, val) { }
QVector3D& CVertexes::operator ()(int p) {
    return *static_cast<QVector3D*>(static_cast<void*>(this->data() + p * 3));
}
const QVector3D& CVertexes::operator ()(int p) const {
    return *static_cast<const QVector3D*>(static_cast<const void*>(this->data() + p * 3));
}


RealRange CVertexes::estimateRange() const {
    RealRange domain((*this)(0).lengthSquared());
    int i(this->length());
    while(i) {
        domain.include((*this)(--i).lengthSquared());
    }
    domain.setMin(sqrt(domain.getMin()));
    domain.setMax(sqrt(domain.getMax()));
    return domain;
}
IndexRange CVertexes::estimateRangeIndex() const {
    RealRange domain((*this)(0).lengthSquared());
    IndexRange range(0);
    int i(this->length());
    while(i) {
        float v((*this)(--i).lengthSquared());
        if (v < domain.getMin()) {
            domain.setMin(v);
            range.setMin(i);
        } else if (v > domain.getMax()) {
            domain.setMax(v);
            range.setMax(i);
        }
    }
    return range;
}

QDataStream& operator << (QDataStream& out, const CVertexes& v) {
    out << v.size();
    out.writeRawData(static_cast<const char*>(static_cast<const void*>(v.data())), v.size() * sizeof(CVertexes::value_type));
    return out;
}

QDataStream& operator >> (QDataStream& in, CVertexes& v) {
    size_t s;
    in >> s;
    v.resize(s);
    in.readRawData(static_cast<char*>(static_cast<void*>(v.data())), s * sizeof(CVertexes::value_type));
    return in;
}


}