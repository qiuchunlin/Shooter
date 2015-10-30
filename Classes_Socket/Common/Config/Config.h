#ifndef _COMMON_CONFIG_CONFIG_
#define _COMMON_CONFIG_CONFIG_
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
namespace Config{
	enum GameType
	{
		Game_Normal,
		Game_GoldIsland,
		Game_EvilIsland
	};

	enum ShopPackage
	{
		Package_S = 1000,
		Package_M = 5000,
		Package_L = 20000,
		Package_XL = 100000,
	};

	enum TouchMode{
		OnePoint,
		VerticalOne,
		HoldPoint,
		Horizontal,
		MoveCloser,
		VerticalTwo
	};

	enum GameLocation
	{
		Pos_LeftUp,
		Pos_RightUp,
		Pos_Left,
		Pos_Right,
		Pos_LeftBottom,
		Pos_CenterBottom,
		Pos_RightBottom,
	};

	enum AttackLocation
	{
		Atk_Pos_CenterUp,
		Atk_Pos_LeftUp,
		Atk_Pos_RightUp,
		Atk_Pos_LeftDown,
		Atk_Pos_RightDown,
		Atk_Pos_Center
	};

	enum MonsterType
	{
		Type_Near,//近战
		Type_Explode,//自爆兵
		Type_Tank,//坦克
		Type_Medical,//医疗兵
		Type_Far,//远程
		Type_Rush,//冲锋
		Type_Carray,//召唤
		Type_Near_Advance,//高级近战
		Type_Award,//哥布林
		Type_Map_Destory,//地图障碍物

		Type_Near_1 = 10,//近战
		Type_Explode_1 = 11,//自爆兵
		Type_Tank_1 = 12,//坦克
		Type_Medical_1 = 13,//医疗兵
		Type_Far_1 = 14,//远程
		Type_Rush_1 = 15,//冲锋
		Type_Carray_1 = 16,//召唤
		Type_Near_Advance_1 = 17,//高级近战
		Type_Award_1 = 18,//哥布林

		Type_Near_2 = 20,//近战
		Type_Explode_2 = 21,//自爆兵
		Type_Tank_2 = 22,//坦克
		Type_Medical_2 = 23,//医疗兵
		Type_Far_2 = 24,//远程
		Type_Rush_2 = 25,//冲锋
		Type_Carray_2 = 26,//召唤
		Type_Near_Advance_2 = 27,//高级近战
		Type_Award_2 = 28,//哥布林
	};

	enum MonsterStatus
	{
		Status_Waite,//等待
		Status_Run,//冲刺
		Status_Attack,//正向攻击
		Status_Dead,//死亡
	};

	enum DamageType
	{
		Damage_Normal,//普通伤害
		Damage_Thunder,//雷电伤害
		Damage_Fire,//火伤害
	};

	//英雄Debuff状态
	enum DebuffType
	{
		Debuff_Dizzy,//晕眩
		Debuff_Blood,//不停流血
		Debuff_SlowDown,//减速
		Debuff_EasyHurt,//附加伤害
		Debuff_Mess,//混乱
		Debuff_Weak,//削弱
	};

	//加载界面类型
	enum LoadingType
	{
		Loading_Cover,
		Loading_Game
	};

	//加载类型
	enum HttpLoadingType
	{
		Loading_Rank,
		Loading_UserInfo,
		Loading_Research
	};

	//新手引导类型
	enum TutorType
	{
		No_Tutor = 0,
		Tutor_Mission0_1 = 1,
		Tutor_Mission0_2 = 2,
		Tutor_Mission0_3 = 3,
		Tutor_Mission0_4 = 4,
		Tutor_Mission0_5 = 5,
		Tutor_Mission0_6 = 6,
		Tutor_Mission0_7 = 7,
		Tutor_Mission0_8 = 8,
		Tutor_Mission0_9 = 9,
		Tutor_Mission0_10 = 10,
		Tutor_Mission0_11 = 11,
		Tutor_Mission0_12 = 12,
		Tutor_Mission0_13 = 13,
		Tutor_Mission0_14 = 14,
		Tutor_Mission0_15 = 15,
		Tutor_Mission0_16 = 16,
		Tutor_Mission0_17 = 17,
		Tutor_Mission0_18 = 18,
		Tutor_Mission0_19 = 19,
		Tutor_Mission0_20 = 20,
		Tutor_Mission0_21 = 21,
		Tutor_Mission0_22 = 22,
		Tutor_Mission0_23 = 23,
		Tutor_Mission0_24 = 24,
		Tutor_Mission0_25 = 25,
		Tutor_Mission0_26 = 26,
		Tutor_Mission0_27 = 27,

		Tutor_Mission1_1 = 30,
		Tutor_Mission1_2 = 32,
		Tutor_Mission1_3 = 35,
		Tutor_Mission1_4 = 40,
		Tutor_Mission1_5 = 50,
		Tutor_Mission1_6 = 60,
		Tutor_Mission1_7 = 70,
		Tutor_Mission1_7_1 = 71,
		Tutor_Mission1_8 = 80,
		Tutor_Mission1_9 = 90,
		Tutor_Mission1_10 = 100,
		Tutor_Mission1_11 = 110,
		Tutor_Mission1_12 = 120,
		Tutor_Mission1_13 = 130,
		Tutor_Mission2_1 = 140,
		Tutor_Mission2_2 = 150,
		Tutor_Mission2_3 = 160,
		Tutor_Mission2_4 = 170,
		Tutor_Mission2_5 = 180,

		Tutor_Mission2_6 = 181,
		Tutor_Mission2_7 = 182,
		Tutor_Mission2_8 = 183,
		Tutor_Mission2_9 = 184,
		Tutor_Mission2_10 = 185,
		Tutor_Mission2_11 = 186,
		Tutor_Mission2_12 = 187,
		Tutor_Mission2_13 = 188,
		Tutor_Mission2_14 = 189,
		Tutor_Mission2_15 = 190,
		Tutor_Mission2_16 = 191,
		Tutor_Mission2_17 = 192,
		Tutor_Mission2_18 = 193,
		Tutor_Mission2_19 = 194,
		Tutor_Mission2_20 = 195,
		Tutor_Mission2_21 = 196,
		Tutor_Mission2_22 = 197,
		Tutor_Mission2_23 = 198,
		Tutor_Mission2_24 = 199,

		Tutor_Mission3_1 = 200,
		Tutor_Mission4_1 = 210,
		Tutor_Mission5_1 = 220,
		Tutor_Mission6_1 = 230,
	};

	//多倍道具类型
	enum ItemType
	{
		Multi_Coin,
		Multi_Game_Exp,
		Multi_Base_Exp,
		Multi_Skill_Exp
	};
	//多倍道具种类
	enum ItemShopType
	{
		Double_Coin_C_1 = 0,//双倍金币1次
		Double_Coin_C_2 = 1,//双倍金币10次 
		Double_Coin_C_3 = 2,//双倍金币30次
		Double_Coin_T_1 = 3,//双倍金币1小时
		Double_Coin_T_2 = 4,//双倍金币1天
		Double_Coin_T_3 = 5,//双倍金币3天

		Double_Exp_C_1 = 6,//双倍精华1次
		Double_Exp_C_2 = 7,//双倍精华10次
		Double_Exp_C_3 = 8,//双倍精华30次
		Double_Exp_T_1 = 9,//双倍精华1小时
		Double_Exp_T_2 = 10,//双倍精华1天
		Double_Exp_T_3 = 11,//双倍精华3天
	};

	//道具类型
	enum ShopItemType
	{
		Lottery_Coin = 0,//金币
		Lottery_Crystal = 1,//魔钻
		Lottery_Double_Coin_Count = 2,//双倍金币次数
		Lottery_Double_Coin_Time = 3,//双倍金币时间
		Lottery_Double_Exp_Count = 4,//双倍精华次数
		Lottery_Double_Exp_Time = 5,//双倍精华时间
		Lottery_Card = 6,//行动力
		Lottery_GoldKey = 7,//黄金钥匙
		Lottery_StraightLoadGift = 8,//连登礼包
		Lottery_FirstGift = 9,//首冲礼包
		Lottery_TopGift = 10,//至尊礼包
		Lottery_LotteryStone1 = 11,//命运之晶I
		Lottery_LotteryStone2 = 12,//命运之晶II
		Lottery_LotteryStone3 = 13,//命运之晶III
		Lottery_LotteryStone4 = 14,//命运之晶IV
		Lottery_LotteryStone5 = 15,//命运之晶V
		Lottery_VIP_1 = 16,//白银会员周卡
		Lottery_VIP_2 = 17,//黄金会员周卡
		Lottery_VIP_3 = 18,//白金会员周卡
		Lottery_VIP_4 = 19,//钻石会员周卡
		Lottery_Hero = 20,//英雄
		Lottery_BaseLv = 21,//基地等级
		Lottery_Exp = 22,//精华
		Lottery_SkillLv = 23,//阶位
		Lottery_FinishMission = 24,//最大完成关卡
		Lottery_EvilKey = 25,//恶魔岛讨伐令
		Lottery_Crusade_Card = 26
	};
	

	//成就类型
	enum AchieveType
	{
		Achieve_Kill_Monster,//杀怪数量 
		Achieve_Finish_Mission,//关卡节点
		Achieve_Get_Coins, // 获得金币数量
		Achieve_Get_Stars,//获得星星数
		Achieve_Base_Lv,//基地等级
		Achieve_Win_Count,//通关次数
		Achieve_Lose_Count,//战败次数
		Achieve_Buy_Coin,//购买金币数量
		Achieve_Buy_Crystal,//购买水晶数量
	};

	//日常任务类型
	enum EveryDayTaskType
	{
		Task_Win_Count,//胜利次数
		Task_Kill_AwardMonsters,//杀死黄金猎手个数
		Task_Kill_Monsters,//杀死普通怪物个数
		Task_ThreeStars,//完成三星游戏次数
		Task_Upgrade_Heros,//升级英雄次数
		Task_EvilIsLands,//恶魔岛次数
		Task_Lotterys,//祈福次数
		Task_GoldIsLands,//黄金岛次数
		Task_Ktplay,//Ktplay任务类型
		Task_AutoFight,//进行一次探险
		Task_BuyInShop,//在金币商店购买道具
	};

	//排行榜类型
	enum LeaderboardType
	{
		FightPower,
		BaseLv,
		StarCount,
		EvilCount,
		AchieveCount
	};

	//能量球类型
	enum EngeryType
	{
		EngeryWind = 1,
		EngeryLight = 2,
		EngeryWater = 3,
		EngeryDark = 4,
		EngeryFire = 5,
		EngeryThunder = 6,
	};

	//Map类型
	enum MapType
	{
		Map_0,
		Map_1,
		Map_2,
		Map_3,
		Map_4,
		Map_5,
		Map_6,
		Map_Gold_1 = 10,
		Map_Evil = 20,
	};
	//购买类型
	enum ShopBuyType
	{
		BuyType_RMB, //人民币
		BuyType_Crystal,//水晶
		BuyType_Coin,//金币
	};

	class MonsterWave:public Ref
	{
	public:
		int nWaveId = 0;
		GameLocation emLocation = Pos_CenterBottom;
		MonsterType emMonsterType = Type_Near;
		int nMonsterLv = 1;
		int nQueneNum = 0;
		float fAppearTime = 0;
		bool bIsDeadAll = false;
		float fFinishTime = 0;
		float fMoveSpeed = 0;
	};

	class MonsterConfig :public Sprite
	{
	public:
		int nMonsterLV = 1;
		MonsterType emMonsterType = Type_Near;
		int nHP = 0;
		int nArmor = 0;
		float fFR = 0;
		float fER = 0;
		float fDestory = 0;
		int nScore = 0;
		float fAttackSpeed = 0;
		int nExp = 0;
	};

	class SkillConfig :public Node
	{
	public:
		int nSkillId = 0;
		int nSkillLV = 1;
		float fDamage = 0;
		float fDuration = 0;
		DamageType emDamageType = Damage_Normal;
		float fCoolDownTime = 0;
	};

	class HeroUpgradeConfig :public Ref
	{
	public:
		int nHeroLv = 0;
		int nCostExp = 0;
		float nCostCoin = 0;
		int nCostItemId = 0;
		float fHeroAttack = 0;
	};

	class BaseUpgradeConfig :public Ref
	{
	public:
		int nBaseLv = 0;
		int nCostExp = 0;
	};

	class GameTipConfig :public Ref
	{
	public:
		int  nDirection = 0;
		int nAppearMode = 0;
		string sName = "";
		string sHeadUrl = "";
		string sContent = "";

		string sBgImgUrl = "";
		string sWordImg1Url = "";
		string sWordImg2Url = "";
		string sDialogContent = "";
		Vec2 tipPos = Vec2::ZERO;
	};

	class HeroConfig : public Ref
	{
	public:
		int nID = -1;
		string sName = "";
		int nType = -1;
		int nStars = -1;
		string sSkillOneName = "";
		string sSkillOneDetail = "";
		string sSkillTwoName = "";
		string sSkillTwoDetail = "";
		string sIntroDuction = ""; 
	};

	class MonsterInfoConfig : public Ref
	{
	public:
		MonsterType eType = Type_Near;
		string sName = "";
		int nAttackStar = 0;
		int nHpStar = 0;
		string sSpecial = "";
	};

	class LevelRewardItem : public Ref
	{
	public:
		int nItemId = 0;
		string sItemIcon = "";
		int nItemNum = 1;
	};

	class LevelRewardConfig : public Ref
	{
	public:
		int nLevel = 0;
		Vector<LevelRewardItem*> vLevelRewardItems;
		bool isHave = false;
		bool isGet = false;
	};

	
	class MagicBallRandomConfig : public Ref
	{
	public:
		MonsterType emType;
		int nPercent;
	};

	class AchieveConfig : public Node
	{
	public:
		string sName = "";
		string sDescribe = "";
		AchieveType emType;
		EveryDayTaskType emAveryDayType;
		int nCondition = 0;
		int nAwardCoin = 0;
		int nAwardCrystal = 0;
		int nAwardBaseExp = 0;
		int nAwardGameExp = 0;
		int nPoint = 0;
	};

	class MissionInfoConfig : public Ref
	{
	public:
		int nId = 0;
		string sName = "";
		string sDescribe = "";
		string sGuide = "";
		vector<string> vMonsters;
		vector<string> vAwards;
	};

	class KtUser : public Ref
	{
	public:
		string sUserId = "";
		string sNickname = "";
		string sHeaderUrl = "";
		string sRank = "";
		string sScore = "";
		string sBaseLv = "";
		string sFightValue = "";
		bool bIsFriend = false;
	};

	class LotteryItem : public Ref
	{
	public:
		int nId = 0;
		ShopItemType emType;
		string sIcon = "";
		float  fCondition = 0;
		float fProbability = 0;
	};

	class LoginReward : public Ref
	{
	public:
		struct LoginRewardItem
		{
			ShopItemType emType;
			string sIcon = "";
			int   nAwardNum = 0;
		};

	public:
		int nId = 0;
		string sGiftIcon = "";
		vector<LoginRewardItem> _vLoginRewardItems;
	};

	class ShopItem : public Ref
	{
	public:
		int nId = 0;
		ShopItemType emType;
		int nNum = 0;
		int nLimit = 0;
		ShopBuyType emBuyType;
		string sIcon = "";
		string sName = "";
		float nPrice = 0;
		string sDescribe = "";
		int nSoldType = 0;
		
	};

	class InfoItem :public Ref
	{
	public:
		int nItemId = 0;
		string sItemName = "";
		int nItemHeroId = 0;
		string sItemDescription = "";
	};

	class AutoAttackRandomRatio :public Ref
	{
	public:
		string sDescription = "";
		float fRatio = 0.0f;
	};

	class MissionItem : public Ref
	{
	public:
		ShopItemType emType;
		string sIcon = "";
		int nItemProb = 0;
		int nNum = 0;
	};

}
#endif
