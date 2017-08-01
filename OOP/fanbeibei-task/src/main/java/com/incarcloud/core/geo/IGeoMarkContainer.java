package com.incarcloud.core.geo;/**
 * Created by fanbeibei on 2017/7/31.
 */

import com.incarcloud.core.IBound;
import com.incarcloud.core.IMark;
import com.incarcloud.core.IMarkContainer;

import java.util.List;

/**
 * @author Fan Beibei
 * @Description:
 * @date 2017/7/31 16:38
 */
public interface IGeoMarkContainer extends IMarkContainer<IGeoMark,IGeoBound> {
    @Override
    List<IGeoMark> getLastMarkInsideBound(IGeoBound bound) throws Exception;
}
