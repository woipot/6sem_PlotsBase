#ifndef COLORGETTER_H
#define COLORGETTER_H

#include <QColor>
#include <QVector>

class ColorGetter
{
    QVector<QColor> colours_ = {QColor("cyan"), QColor("magenta"), QColor("red"),
                              QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                              QColor("green"), QColor("darkGreen"), QColor("yellow"),
                              QColor("blue")};
   int iterator_ = 0;
public:

    QColor get_nextColor()
    {
        if (iterator_ == colours_.count())
            iterator_ = 0;

        return colours_[iterator_++];
    }
};

#endif // COLORGETTER_H
