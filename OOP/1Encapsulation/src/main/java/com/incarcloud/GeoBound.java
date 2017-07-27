package com.incarcloud;

class GeoBound {
    private double _left;
    private double _right;
    private double _top;
    private double _bottom;

    GeoBound(double left, double top, double right, double bottom)
    {
        _left = left;
        _right = right;
        _top = top;
        _bottom = bottom;
    }

    boolean isInside(double lon, double lat)
    {
        return (lon >= _left && lon <= _right && lat >= _bottom && lat <= _top);
    }
}
