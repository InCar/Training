package com.incarcloud.core.geo;/**
 * Created by fanbeibei on 2017/7/31.
 */

import com.incarcloud.core.IBound;

/**
 * @author Fan Beibei
 * @Description: GEO边界
 * @date 2017/7/31 15:13
 */
public interface IGeoBound extends IBound<IGeoMark> {
    @Override
    boolean isInside(IGeoMark lastMark);

    /**
     * 西部经度边界
     * @return
     */
    public double getWestLon();
    /**
     * 东部经度边界
     * @return
     */
    public double getEastLon();
    /**
     * 北部纬度边界
     * @return
     */
    public double getNorthLat();
    /**
     * 南部纬度边界
     *
     * @return
     */
    public double getSouthLat();
}
