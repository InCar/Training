package com.incarcloud.my.geo;/**
 * Created by fanbeibei on 2017/7/31.
 */

import com.incarcloud.core.geo.IGeoBound;
import com.incarcloud.core.geo.IGeoMark;

/**
 * @author Fan Beibei
 * @Description: 描述
 * @date 2017/7/31 14:30
 */
public class MyGeoBound implements IGeoBound {
    /**
     * 西部经度边界
     */
    private double westLon;
    /**
     * 东部经度边界
     */
    private double eastLon;
    /**
     * 北部纬度边界
     */
    private double northLat;
    /**
     * 南部纬度边界
     */
    private double southLat;

    /**
     * @param westLon  西部经度边界
     * @param eastLon  东部经度边界
     * @param northLat 北部纬度边界
     * @param southLat 南部纬度边界
     */
    public MyGeoBound(double westLon, double eastLon, double northLat, double southLat) {
        this.westLon = westLon;
        this.eastLon = eastLon;
        this.northLat = northLat;
        this.southLat = southLat;
    }

    @Override
    public boolean isInside(IGeoMark lastMark) {
        if (null == lastMark) {
            throw new IllegalArgumentException();
        }


        double lon = lastMark.getLon();
        double lat = lastMark.getLat();
        return (lon >= westLon && lon <= eastLon && lat >= southLat && lat <= northLat);
    }


    @Override
    public double getWestLon() {
        return westLon;
    }

    @Override
    public double getEastLon() {
        return eastLon;
    }

    @Override
    public double getNorthLat() {
        return northLat;
    }

    @Override
    public double getSouthLat() {
        return southLat;
    }
}
