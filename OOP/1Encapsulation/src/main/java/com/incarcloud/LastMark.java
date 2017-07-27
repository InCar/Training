package com.incarcloud;

class LastMark
{
    private String _id;
    private String _tm;
    private double _lon;
    private double _lat;

    LastMark(String id, String tm, double lon, double lat){
        this._id = id;
        this._tm = tm;
        this._lon = lon;
        this._lat = lat;
    }

    boolean isInside(GeoBound bound){
        return bound.isInside(this._lon, this._lat);
    }

    String SayInCity(String city){
        return String.format("%s is in %s at %s", this._id, city, this._tm);
    }
}

