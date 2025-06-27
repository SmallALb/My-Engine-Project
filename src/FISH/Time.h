#pragma once

namespace FISH {
      namespace Time {
        
        extern float DeltaTime;     
        extern float FramePreSecond;
        extern float Step;

        // 获取时间
        float GetTime();
        //设置帧率
        void SetFPS(float FPS);
        //更新帧变化时间
        void updateDt();
    }
}