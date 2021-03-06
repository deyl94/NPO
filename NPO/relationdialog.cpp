#include "relationdialog.h"
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDir>

RelationDialog::RelationDialog(GeometryPair *forEdit, QWidget *parent)
  : QDialog(parent)
  , toggleOff(":/media/images/togleOff.png")
  , toggleOn(":/media/images/togleOn.png")
  , pair(*forEdit)
  , relation(pair.relations())
  , leftL(pair.theory().modesCount())
  , rightL(pair.practic().modesCount())
  , maxW(0)
  , underToggle(-1)
  , underLeftToggle(false)
  , lineingState(-1)
  , lineingLeft(false)
  , title1(new QLabel(forEdit->theory().getName(), this))
  , title2(new QLabel(forEdit->practic().getName(), this))
  , accept(new QPushButton("accept", this))
  , cancel(new QPushButton("cancel", this))
{
    this->setModal(true);
    this->setContentsMargins(10, 0, 10, 0);
    bildLabels(leftL, pair.theory());
    bildLabels(rightL, pair.practic());
    foreach (QLabel* l, leftL)
        l->resize(maxW, l->height());
    foreach (QLabel* l, rightL)
        l->resize(maxW, l->height());

    this->setMinimumSize(maxW * 2 + 120, leftL.first()->height() * (leftL.size() > rightL.size() ? leftL.size() : rightL.size()));
    this->resize(this->minimumWidth() + 200, 650);
    QPalette p(this->palette());
    p.setColor(QPalette::Background, QColor(0xFF,0xFF,0xFF));
    this->setPalette(p);
    this->setMouseTracking(true);

    popupConfig(leftP, leftF, pair.theory());
    popupConfig(rightP, rightF, pair.practic());

    if (relation.size() != leftL.size()) {
        relation.resize(leftL.size());
        for (int i(0); i < relation.size(); ++i) {
            relation[i] = rightL.size() > i ? i : -1;
        }
    }
    bgUpdate();
    this->connect(accept, SIGNAL(clicked()), SLOT(accept()));
    this->connect(cancel, SIGNAL(clicked()), SLOT(accept()));
}

void RelationDialog::bgUpdate()
{
    for (int i(0); i != rightL.size(); ++i)
    {
        rightL[i]->setBackgroundRole(QPalette::Dark);
    }
    for (int i(0); i != leftL.size(); ++i)
    {
        if (relation[i] >= 0)
        {
            rightL[relation[i]]->setBackgroundRole(QPalette::Light);
            leftL[i]->setBackgroundRole(QPalette::Light);
        }
        else
        {
            leftL[i]->setBackgroundRole(QPalette::Dark);
        }
    }
}

void RelationDialog::popupConfig(QFrame*& p, GeometryWidget *&f, const GeometryForm& v)
{
    p = new QFrame(this, Qt::Popup);
    p->setContentsMargins(0,0,0,0);
    p->setFrameStyle(QFrame::Plain | QFrame::StyledPanel);
    p->setLineWidth(1);
    p->setFixedSize(maxW, maxW);
    f = new GeometryWidget(p);
    f->setModel(v);
    p->setLayout(new QHBoxLayout(p));
    p->layout()->setMargin(0);
    p->layout()->addWidget(f);
}

void RelationDialog::bildLabels(Labels &lbls, GeometryForm& g)
{
    QPalette p(this->palette());
    p.setBrush(QPalette::Light, QColor(0xFF,0xFF,0xFF));
    p.setBrush(QPalette::Dark, QColor(0x88,0x88,0x88));
    for (int i = 0; i != lbls.size(); ++i)
    {
        QLabel* l(new QLabel(QString("%1 form (%2hz)").arg(QString::number(i + 1), QString::number(g.frequency(i))),this));
        l->resize(l->sizeHint());
        l->setFrameStyle(QFrame::Plain | QFrame::StyledPanel);
        l->setLineWidth(1);
        l->setCursor(QCursor(Qt::PointingHandCursor));
        l->setContextMenuPolicy(Qt::CustomContextMenu);
        l->installEventFilter(this);
        l->setPalette(p);
        l->setAutoFillBackground(true);
        lbls[i] = l;
        if (maxW < l->sizeHint().width())
            maxW = l->sizeHint().width();
    }
}

bool RelationDialog::eventFilter(QObject* o, QEvent* e)
{
    if (!o || e->type() != QEvent::MouseButtonPress)
        return false;
    QMouseEvent* me(static_cast<QMouseEvent*>(e));
    if (me->button() == Qt::LeftButton)
        showForm(o);
    return true;
}
void RelationDialog::showForm(QObject* s)
{
    QWidget* wgt(0);
    int i, d;
    for (i = 0; i != leftL.size(); ++i)
    {
        if (leftL[i] == s)
            wgt = leftL[d = i];
    }
    if (wgt)
    {
        leftP->move(this->mapToGlobal(QPoint(0,0)) + wgt->geometry().bottomLeft() - QPoint(0,
                                   (wgt->height() + leftP->height()) * (d - 1 > leftL.size() / 2)));
        leftF->setForm(d);
        leftP->show();
        return;
    }
    for (i = 0; i != rightL.size(); ++i)
    {
        if (rightL[i] == s)
            wgt = rightL[d = i];
    }
    if (wgt)
    {
        rightP->move(this->mapToGlobal(QPoint(0,0)) + wgt->geometry().bottomLeft() - QPoint(0,
                                   (wgt->height() + rightP->height()) * (d - 1 > rightL.size() / 2)));
        rightF->setForm(d);
        rightP->show();
        return;
    }
}

void RelationDialog::resizeEvent(QResizeEvent*)
{
    title1->move(this->contentsMargins().left(),5);
    title2->move(this->width() - this->contentsMargins().right() - maxW, 5);
    float myH(leftL.first()->height() / 2.);
    leftCapacity = (this->height() - 20) * 1.0f / leftL.size();
    rightCapacity = (this->height() - 20) * 1.0f / rightL.size();
    for (int i(0); i != leftL.size(); ++i)
    {
        leftL[i]->move(this->contentsMargins().left(), leftCapacity * (i + .5f) - myH + 20);
    }
    for (int i(0); i != rightL.size(); ++i)
    {
        rightL[i]->move(this->width() - this->contentsMargins().right() - maxW, rightCapacity * (i + .5f) - myH + 20);
    }
    accept->move(this->width() / 2. - accept->width() - 10
               , this->height() - accept->height() - 5);
    cancel->move(this->width() / 2. + 10
               , this->height() - accept->height() - 5);
}

void RelationDialog::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(p.pen());
    pen.setStyle(Qt::DashDotDotLine);
    pen.setWidth(3);
    pen.setColor(QColor(0x82,0x87,0x90));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    //p.fillRect(e->rect(), QColor(rand() % 0x100, rand() % 0x100, rand() % 0x100));
    p.fillRect(e->rect(), QColor(0xFF, 0xFF, 0xFF));
    GeometryPair::Relation& relation(pair.relations());
    for (int i = 0; i != pair.relations().size(); ++i)
    {
        if (relation[i] >= 0)
        {
            p.drawLine(this->contentsMargins().left() + maxW + 24, leftCapacity * (i + .5f) + 20,
                       this->width() - this->contentsMargins().right() - maxW - 24, rightCapacity * (relation[i] + .5f) + 20);
        }
    }
    pen.setColor(QColor(0x88,0xFF, 0x88));
    p.setPen(pen);
    if (lineingState >= 0)
    {
        p.drawLine(source, tracking);
    }
    pen.setColor(QColor(0x82,0x87,0x90));
    p.setPen(pen);
    for (int i = 0; i != leftL.size(); ++i) {
        p.drawImage(this->contentsMargins().left() + maxW, leftCapacity * (i + .5f) - toggleOff.height() / 2 + 20,
                    (underToggle == i && underLeftToggle) ||
                    (lineingState == i && lineingLeft) ? toggleOn : toggleOff);
    }
    p.rotate(180);
    p.translate(-this->width(),-this->height());
    for (int i(rightL.size()); i; )
    {
        --i;
        p.drawImage(this->contentsMargins().right() + maxW, rightCapacity * (i + .5f) - toggleOff.height() / 2 - 0,
                    (underToggle == rightL.size() - i - 1 && !underLeftToggle) ||
                    (lineingState == rightL.size() - i - 1 && !lineingLeft) ? toggleOn :  toggleOff);
    }
    p.rotate(180);
    p.translate(-this->width(),-this->height());
}

void RelationDialog::mouseMoveEvent(QMouseEvent *e)
{
    tracking = e->pos();

    const QPoint& p(e->pos());
    QRectF crnt(QPointF(this->contentsMargins().left() + maxW, (leftCapacity - toggleOff.height()) / 2 + 20), toggleOff.size());
    int i = -1;
    bool side(false);
    while (++i != leftL.size() && !crnt.contains(p))
    {
        crnt.translate(0, leftCapacity);
    }
    if (i == leftL.size())
    {
        crnt.moveTo(this->width() - maxW - this->contentsMargins().right() - toggleOff.width(),(rightCapacity - toggleOff.height()) / 2 + 20);
        i = -1;
        while (++i != rightL.size() && !crnt.contains(p))
        {
            crnt.translate(0, rightCapacity);
        }
        if (i == rightL.size())
        {
            i = -1;
        }
    }
    else
    {
        side = true;
    }
    if (lineingState >= 0 || i != underToggle || (underLeftToggle != side && i != -1))
    {
        this->setCursor(QCursor(i >= 0 ? Qt::PointingHandCursor : Qt::ArrowCursor));
        underToggle = i;
        underLeftToggle = side;
        updateLines();
    }
}

void RelationDialog::updateLines()
{
    this->update(
        QRect(QPoint(leftL.first()->geometry().right(), (leftCapacity > rightCapacity ? rightL : leftL).first()->geometry().top()),
              QPoint(rightL.last()->geometry().left(), (leftCapacity > rightCapacity ? rightL : leftL).last()->geometry().bottom())));
}

void RelationDialog::mousePressEvent(QMouseEvent * e)
{
    if (underToggle >= 0 && lineingState < 0)
    {
        lineingState = underToggle;
        lineingLeft = underLeftToggle;
        if (underLeftToggle)
        {
            relation[underToggle] = -1;
            source = QPoint(this->contentsMargins().left() + maxW + 24, leftCapacity * (underToggle + .5f) + 20);
        }
        else
        {
            *qFind(relation.begin(), relation.end(), underToggle) = -1;
            source = QPoint(this->width() - this->contentsMargins().right() - maxW - 24, rightCapacity * (underToggle + .5f) + 20);
        }
    }
    else if (underToggle >= 0 && underLeftToggle != lineingLeft)
    {
        if (underLeftToggle)
        {
            relation[underToggle] = lineingState;
        }
        else
        {
            GeometryPair::Relation::iterator esc(qFind(relation.begin(), relation.end(), underToggle));
            if (esc != relation.end())
                *esc = -1;
            relation[lineingState] = underToggle;
        }
        lineingState = -1;
    }
    else if (lineingState >= 0)
    {
        lineingState = -1;
    }
    bgUpdate();
    updateLines();
}

void RelationDialog::run(GeometryPair* forEdit, QWidget* parent)
{
    QEventLoop* l(new QEventLoop(parent));
    RelationDialog* dialog(new RelationDialog(forEdit, parent));
    qDebug() << "construct";
    l->connect(dialog, SIGNAL(finished(int)), SLOT(quit()));
    dialog->show();
    l->exec();
    qDebug() << "result";
}
