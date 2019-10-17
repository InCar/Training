package com.incarcloud.taskgo;

import com.incarcloud.lang.Action;

import java.util.Random;

class CpuTask implements Action<Integer> {
    private float _fSum = 0.0f;
    private Random _rand = new Random();

    @Override
    public void run(Integer count) {
        for(int i=0;i<count;i++){
            _fSum += _rand.nextInt(100);
        }
    }
}
