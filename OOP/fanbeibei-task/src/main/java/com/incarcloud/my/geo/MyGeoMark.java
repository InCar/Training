package com.incarcloud.my.geo;/**
 * Created by fanbeibei on 2017/7/31.
 */


import com.incarcloud.core.geo.IGeoBound;
import com.incarcloud.core.geo.IGeoMark;

import java.util.Date;

/**
 * @author Fan Beibei
 * @Description: 边界线接口实现
 * @date 2017/7/31 14:02
 */
public class MyGeoMark implements IGeoMark {
    /**
     * ID
     */
    private String id;
    /**
     * 时间戳
     */
    private Date timestamp;
    /**
     * 经度
     */

    private double lon;
    /**
     * 纬度
     */
    private double lat;

    /**
     * @param id        ID
     * @param timestamp 时间戳
     * @param lon       经度
     * @param lat       纬度
     */
    public MyGeoMark(String id, Date timestamp, double lon, double lat) {
        this.id = id;
        this.timestamp = timestamp;
        this.lon = lon;
        this.lat = lat;
    }

    @Override
    public double getLat() {
        return lat;
    }

    @Override
    public double getLon() {
        return lon;
    }

    @Override
    public String getId() {
        return id;
    }

    @Override
    public Date getTimestamp() {
        return timestamp;
    }

    @Override
    public boolean isInside(IGeoBound bound) {
        return bound.isInside(this);
    }

//    @Override
//    public String SayInCity(String city){
//       return String.format("%s is in %s at %s", this.id, city, this.tm);
//    }

}
