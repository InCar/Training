package com.incarcloud.core;/**
 * Created by fanbeibei on 2017/7/31.
 */

import com.incarcloud.my.geo.MyGeoBound;

import java.util.List;

/**
 * @author Fan Beibei
 * @Description: 位置容器
 * @date 2017/7/31 16:36
 */
public interface IMarkContainer<M extends  IMark,B extends  IBound> {
    List<M> getLastMarkInsideBound(B bound) throws Exception;
}
