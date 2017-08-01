package com.incarcloud.core;/**
 * Created by fanbeibei on 2017/7/31.
 */

import java.util.Date;

/**
 * @author Fan Beibei
 * @Description: 位置接口
 * @date 2017/7/31 14:39
 */
public interface IMark<T extends IBound> {
    /**
     * ID
     * @return
     */
    String getId();

    /**
     * 获取时间戳
     * @return
     */
    Date getTimestamp();

    /**
     * 是否在区域内
     *
     * @param bound 区域边界
     * @return
     */
    boolean isInside(T bound);
}
