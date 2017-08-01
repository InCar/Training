package com.incarcloud.core.geo;/**
 * Created by fanbeibei on 2017/7/31.
 */


import com.incarcloud.core.IMark;

/**
 * @author Fan Beibei
 * @Description: GEO位置接口
 *
 * @date 2017/7/31 10:54
 */
public interface IGeoMark extends IMark<IGeoBound> {

    @Override
    boolean isInside(IGeoBound bound);

    /**
     * 经度
     * @return
     */
    double getLon();

    /**
     * 纬度
     * @return
     */
    double getLat();

    /**
     * 打印 是否在城市内 的文案
     * @param city 城市名称
     * @return
     */
//    String SayInCity(String city);

}
