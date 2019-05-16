# game

Phase1: basic scene development:
    1.loging 
    2.main scene (for starting game buttom)
    3.mode select scene (single or multiply)
    4.score board (durting the game)
    
    • 登陆界面
        ○ 触发事件：程序加载
            i. 启动游戏后显示加载画面
            ii. 播放开始动画？加载京都条
    • 进入游戏界面
        ○ 触发：结束加载
        ○ 按钮：开始游戏
    • 选择模式界面：
        ○ 触发：点击开始游戏
        ○ 按钮：单人模式/双人模式？？？？
        
    • 玩家计分牌：
        ○ 触发：开始游戏。


Phase2 : Game machenism development

    • 玩家操作规则
        ○ 发射子弹
            § 发射方式：触摸事件，鼠标点击时间
            § 发射频率：初始间隔1发/s
            § 飞行距离：相对位置5dpi
            § 子弹威力：初始威力2
        ○ 移动玩家
                § 移动方式：虚拟手柄，键盘操作
                § 移动方向：360度
                § 移动速度：初始数值2dpi/s？但随等级提高而减小
        ○ 死亡
            § 生命值小于零
            § 死亡结果：游戏结束--提示是否购买生命--重新开始or回到主界面
        ○ 获取道具
            § 获取方式：进行物理碰撞，某些角色可以在一定范围内发生碰撞（磁吸效果）
            § 获取diomend：经验值+5
            § 获取heart：生命值+1
        ○ 玩家升级
            § 触发机制：经验值到达某一数值
            § 获取机制：1.拣取diomend（+5） 2.击杀玩家（+20）
            § 触发事件：升级动画，并获得升级技能
    
    使用软件建立基本素材：
        场景 cocosbuilder
        精灵 darkfunction editor
        动作 animatepacker
        粒子系统 particle designer
        音效 CFXR


