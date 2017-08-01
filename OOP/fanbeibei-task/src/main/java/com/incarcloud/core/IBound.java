package com.incarcloud.core;/**
 * Created by fanbeibei on 2017/7/31.
 */

/**
 * @author Fan Beibei
 * @Description: 区域边界接口
 * @date 2017/7/31 10:56
 */
public interface IBound<T extends  IMark> {
    /**
     * 是否在区域内
     * @param lastMark 最后标记的地址
     * @return
     */
     boolean isInside(T lastMark);
}
