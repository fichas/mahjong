
/*** Start of inlined file: tile.h ***/
#ifndef __MAHJONG_ALGORITHM__TILE_H__
#define __MAHJONG_ALGORITHM__TILE_H__

#include <stddef.h>
#include <stdint.h>

 // force inline
#ifndef FORCE_INLINE
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) && ((__GNUC__ << 8 | __GNUC_MINOR__) >= 0x301)
#define FORCE_INLINE __inline__ __attribute__((__always_inline__))
#else
#define FORCE_INLINE inline
#endif
#endif

 // unreachable
#ifndef UNREACHABLE
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#define UNREACHABLE() __assume(0)
#elif defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ << 8 | __GNUC_MINOR__) >= 0x405))
#define UNREACHABLE() __builtin_unreachable()
#else
#define UNREACHABLE() assert(0)
#endif
#endif

namespace mahjong {

/**
 * @brief 代码注释中用到的术语简介
 * - 顺子：数牌中，花色相同序数相连的3张牌。
 * - 刻子：三张相同的牌。碰出的为明刻，未碰出的为暗刻。俗称坎。杠也算刻子，明杠算明刻，暗杠算暗刻。
 * - 面子：顺子和刻子的统称。俗称一句话、一坎牌。
 * - 雀头：基本和牌形式中，单独组合的对子，也叫将、眼。
 * - 基本和型：4面子1雀头的和牌形式。
 * - 特殊和型：非4面子1雀头的和牌形式，在国标规则中，有七对、十三幺、全不靠等特殊和型。
 * - 门清：也叫门前清，指不吃、不碰、不明杠的状态。特殊和型必然是门清状态。暗杠虽然不破门清，但会暴露出手牌不是特殊和型的信息。
 * - 副露：吃牌、碰牌、杠牌的统称，即利用其他选手打出的牌完成自己手牌面子的行为，一般不包括暗杠，也叫鸣牌，俗称动牌。
 *     副露有时候也包括暗杠，此时将暗杠称为之暗副露，而吃、碰、明杠称为明副露。
 * - 立牌：整个手牌除去吃、碰、杠之后的牌。
 * - 手牌：包括立牌和吃、碰、杠的牌，有时仅指立牌。
 * - 听牌：只差所需要的一张牌即能和牌的状态。俗称下叫、落叫、叫和（糊）。
 * - 一上听：指差一张就能听牌的状态，也叫一向听、一入听。以此类推有二上听、三上听、N上听。
 * - 上听数：达到听牌状态需要牌的张数。
 * - 有效牌：能使上听数减少的牌，也称进张牌、上张牌。
 * - 改良牌：能使有效牌增加的牌。通俗来说就是能使进张面变宽的牌。
 * - 对子：两张相同的牌。雀头一定是对子，但对子不一定是雀头。
 * - 两面：数牌中，花色相同数字相邻的两张牌，如45m，与两侧的牌都构成顺子。也叫两头。
 * - 嵌张：数牌中，花色相同数字相隔1的两张牌，如57s，只能与中间的牌构成顺子，中间的这张牌称为嵌张。
 * - 边张：也是数字相邻的两张牌，但由于处在边界位置，只能与一侧的牌能构成顺子，如12只能与3构成顺子、89只能与7构成顺子，这张3或者7便称为边张。
 * - 搭子：指差一张牌就能构成1组面子的两张牌。其形态有刻子搭子（即对子）、两面搭子、嵌张搭子、边张搭子。
 * - 复合搭子：多张牌构成的搭子。常见的有：连嵌张、两面带对子、嵌张带对子、边张带对子等等形态。
 * - 对倒：听牌时，其他牌都已经构成面子，剩余两对，只需任意一对成刻即可和牌，此时另一对充当雀头，这种听牌形态叫对倒，也叫双碰、对碰、对杵。
 */

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief 花色
 */
typedef uint8_t suit_t;

/**
 * @brief 点数
 */
typedef uint8_t rank_t;

#define TILE_SUIT_NONE          0  ///< 无效
#define TILE_SUIT_CHARACTERS    1  ///< 万子（CHARACTERS）
#define TILE_SUIT_BAMBOO        2  ///< 条子（BAMBOO）
#define TILE_SUIT_DOTS          3  ///< 饼子（DOTS）
#define TILE_SUIT_HONORS        4  ///< 字牌（HONORS）

/**
 * @brief 牌\n
 * 内存结构：
 * - 0-3 4bit 牌的点数
 * - 4-7 4bit 牌的花色
 * 合法的牌为：
 * - 0x11 - 0x19 万子（CHARACTERS）
 * - 0x21 - 0x29 条子（BAMBOO）
 * - 0x31 - 0x39 饼子（DOTS）
 * - 0x41 - 0x47 字牌（HONORS）
 * - 0x51 - 0x58 花牌（FLOWER）
 */
typedef uint8_t tile_t;

/**
 * @brief 生成一张牌
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] suit 花色
 * @param [in] rank 点数
 * @return tile_t 牌
 */
static FORCE_INLINE tile_t make_tile(suit_t suit, rank_t rank) {
	return (((suit & 0xF) << 4) | (rank & 0xF));
}

/**
 * @brief 获取牌的花色
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile 牌
 * @return suit_t 花色
 */
static FORCE_INLINE suit_t tile_get_suit(tile_t tile) {
	return ((tile >> 4) & 0xF);
}

/**
 * @brief 判断是否为花牌
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_flower(tile_t tile) {
	return ((tile >> 4) & 0xF) == 5;
}

/**
 * @brief 获取牌的点数
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile 牌
 * @return rank_t 点数
 */
static FORCE_INLINE rank_t tile_get_rank(tile_t tile) {
	return (tile & 0xF);
}

/**
 * @brief 所有牌的值，不包括花牌
 */
enum tile_value_t {
	TILE_1m = 0x11, TILE_2m, TILE_3m, TILE_4m, TILE_5m, TILE_6m, TILE_7m, TILE_8m, TILE_9m,
	TILE_1s = 0x21, TILE_2s, TILE_3s, TILE_4s, TILE_5s, TILE_6s, TILE_7s, TILE_8s, TILE_9s,
	TILE_1p = 0x31, TILE_2p, TILE_3p, TILE_4p, TILE_5p, TILE_6p, TILE_7p, TILE_8p, TILE_9p,
	TILE_E  = 0x41, TILE_S , TILE_W , TILE_N , TILE_C , TILE_F , TILE_P ,
	TILE_TABLE_SIZE
};

/**
 * @brief 所有合法的牌，不包括花牌
 */
static const tile_t all_tiles[] = {
	TILE_1m, TILE_2m, TILE_3m, TILE_4m, TILE_5m, TILE_6m, TILE_7m, TILE_8m, TILE_9m,
	TILE_1s, TILE_2s, TILE_3s, TILE_4s, TILE_5s, TILE_6s, TILE_7s, TILE_8s, TILE_9s,
	TILE_1p, TILE_2p, TILE_3p, TILE_4p, TILE_5p, TILE_6p, TILE_7p, TILE_8p, TILE_9p,
	TILE_E , TILE_S , TILE_W , TILE_N , TILE_C , TILE_F , TILE_P
};

/**
 * @brief 牌表类型
 *
 * 说明：在判断听牌、计算上听数等算法中，主流的对于牌有两种存储方式：
 * - 一种是用牌表，各索引表示各种牌拥有的枚数，这种存储方式的优点是在递归计算时削减面子只需要修改表中相应下标的值，缺点是一手牌的总数不方便确定
 * - 另一种是直接用牌的数组，这种存储方式的优点是很容易确定一手牌的总数，缺点是在递归计算时削减面子不方便，需要进行数组删除元素操作
 */
typedef uint16_t tile_table_t[TILE_TABLE_SIZE];

#define PACK_TYPE_NONE 0  ///< 无效
#define PACK_TYPE_CHOW 1  ///< 顺子
#define PACK_TYPE_PUNG 2  ///< 刻子
#define PACK_TYPE_KONG 3  ///< 杠
#define PACK_TYPE_PAIR 4  ///< 雀头

/**
 * @brief 牌组
 *  用于表示一组面子或者雀头
 *
 * 内存结构：
 * - 0-7 8bit tile 牌（对于顺子，则表示中间那张牌，比如234p，那么牌为3p）
 * - 8-11 4bit type 牌组类型，使用PACK_TYPE_xxx宏
 * - 12-13 2bit offer 供牌信息，取值范围为0123\n
 * - 14 1bit promoted 是否为加杠
 *       0表示暗手（暗顺、暗刻、暗杠），非0表示明手（明顺、明刻、明杠）
 *
 *       对于牌组是刻子和杠时，123分别来表示是上家/对家/下家供的\n
 *       对于牌组为顺子时，由于吃牌只能是上家供，这里用123分别来表示第几张是上家供的
 */
typedef uint16_t pack_t;

/**
 * @brief 生成一个牌组
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] offer 供牌信息
 * @param [in] type 牌组类型
 * @param [in] tile 牌（对于顺子，为中间那张牌）
 */
static FORCE_INLINE pack_t make_pack(uint8_t offer, uint8_t type, tile_t tile) {
	return (offer << 12 | (type << 8) | tile);
}

/**
 * @brief 牌组是否为明的
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] pack 牌组
 * @return bool
 */
static FORCE_INLINE bool is_pack_melded(pack_t pack) {
	return !!(pack & 0x3000);
}

/**
 * @brief 牌组是否为加杠
 *  当牌组不是PACK_TYPE_KONG时，结果是无意义的
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] pack 牌组
 * @return bool
 */
static FORCE_INLINE bool is_promoted_kong(pack_t pack) {
	return !!(pack & 0x4000);
}

/**
 * @brief 碰的牌组转换为加杠
 *  当牌组不是PACK_TYPE_PUNG时，结果是无意义的
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] pack 碰的牌组
 * @return pack_t 加杠的牌组
 */
static FORCE_INLINE pack_t promote_pung_to_kong(pack_t pack) {
	return pack | 0x4300;
}

/**
 * @brief 牌组的供牌信息
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] pack 牌组
 * @return uint8_t
 */
static FORCE_INLINE uint8_t pack_get_offer(pack_t pack) {
	return ((pack >> 12) & 0x3);
}

/**
 * @brief 获取牌组的类型
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] pack 牌组
 * @return uint8_t 牌组类型
 */
static FORCE_INLINE uint8_t pack_get_type(pack_t pack) {
	return ((pack >> 8) & 0xF);
}

/**
 * @brief 获取牌的点数
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] pack 牌组
 * @return tile_t 牌（对于顺子，为中间那张牌）
 */
static FORCE_INLINE tile_t pack_get_tile(pack_t pack) {
	return (pack & 0xFF);
}

/**
 * @brief 手牌结构
 *  手牌结构一定满足等式：3*副露的牌组数+立牌数=13
 */
struct hand_tiles_t {
	pack_t fixed_packs[5];      ///< 副露的牌组（面子），包括暗杠
	intptr_t pack_count;        ///< 副露的牌组（面子）数，包括暗杠
	tile_t standing_tiles[13];  ///< 立牌
	intptr_t tile_count;        ///< 立牌数
};

/**
 * @brief 判断是否为绿一色构成牌
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_green(tile_t tile) {
	// 最基本的逐个判断，23468s及发财为绿一色构成牌
	//return (tile == TILE_2s || tile == TILE_3s || tile == TILE_4s || tile == TILE_6s || tile == TILE_8s || tile == TILE_F);

	// 算法原理：
	// 0x48-0x11=0x37=55刚好在一个64位整型的范围内，
	// 用uint64_t的每一位表示一张牌的标记，事先得到一个魔数，
	// 然后每次测试相应位即可
	return !!(0x0020000000AE0000ULL & (1ULL << (tile - TILE_1m)));
}

/**
 * @brief 判断是否为推不倒构成牌
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_reversible(tile_t tile) {
	// 最基本的逐个判断：245689s、1234589p及白板为推不倒构成牌
	//return (tile == TILE_2s || tile == TILE_4s || tile == TILE_5s || tile == TILE_6s || tile == TILE_8s || tile == TILE_9s ||
	//    tile == TILE_1p || tile == TILE_2p || tile == TILE_3p || tile == TILE_4p || tile == TILE_5p || tile == TILE_8p || tile == TILE_9p ||
	//    tile == TILE_P);

	// 算法原理同绿一色构成牌判断函数
	return !!(0x0040019F01BA0000ULL & (1ULL << (tile - TILE_1m)));
}

/**
 * @brief 判断是否为数牌幺九（老头牌）
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_terminal(tile_t tile) {
	// 最基本的逐个判断
	//return (tile == TILE_1m || tile == TILE_9m || tile == TILE_1s || tile == TILE_9s || tile == TILE_1p || tile == TILE_9p);

	// 算法原理：观察数牌幺九的二进制位：
	// 0x11：0001 0001
	// 0x19：0001 1001
	// 0x21：0010 0001
	// 0x29：0010 1001
	// 0x31：0011 0001
	// 0x39：0011 1001
	// 所有牌的低4bit只会出现在0001到1001之间，跟0111位与，只有0001和1001的结果为1
	// 所有数牌的高4bit只会出现在0001到0011之间，跟1100位与，必然为0
	// 于是构造魔数0xC7（1100 0111）跟牌位与，结果为1的，就为数牌幺九
	// 缺陷：低4bit的操作会对0xB、0xD、0xF产生误判，高4bit的操作会对0x01和0x09产生误判
	return ((tile & 0xC7) == 1);
}

/**
 * @brief 判断是否为风牌
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_winds(tile_t tile) {
	return (tile > 0x40 && tile < 0x45);
}

/**
 * @brief 判断是否为箭牌（三元牌）
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_dragons(tile_t tile) {
	return (tile > 0x44 && tile < 0x48);
}

/**
 * @brief 判断是否为字牌
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_honor(tile_t tile) {
	return (tile > 0x40 && tile < 0x48);
}

/**
 * @brief 判断是否为数牌
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_numbered_suit(tile_t tile) {
	if (tile < 0x1A) return (tile > 0x10);
	if (tile < 0x2A) return (tile > 0x20);
	if (tile < 0x3A) return (tile > 0x30);
	return false;
}

/**
 * @brief 判断是否为数牌（更快）
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @see is_numbered_suit
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_numbered_suit_quick(tile_t tile) {
	// 算法原理：数牌为0x11-0x19，0x21-0x29，0x31-0x39，跟0xC0位与，结果为0
	return !(tile & 0xC0);
}

/**
 * @brief 判断是否为幺九牌（包括数牌幺九和字牌）
 * @param [in] tile 牌
 * @return bool
 */
static FORCE_INLINE bool is_terminal_or_honor(tile_t tile) {
	return is_terminal(tile) || is_honor(tile);
}

/**
 * @brief 判断两张牌花色是否相同（更快）
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile0 牌0
 * @param [in] tile1 牌1
 * @return bool
 */
static FORCE_INLINE bool is_suit_equal_quick(tile_t tile0, tile_t tile1) {
	// 算法原理：高4bit表示花色
	return ((tile0 & 0xF0) == (tile1 & 0xF0));
}

/**
 * @brief 判断两张牌点数是否相同（更快）
 *  函数不检查输入的合法性。如果输入不合法的值，将无法保证合法返回值的合法性
 * @param [in] tile0 牌0
 * @param [in] tile1 牌1
 * @return bool
 */
static FORCE_INLINE bool is_rank_equal_quick(tile_t tile0, tile_t tile1) {
	// 算法原理：低4bit表示花色。高4bit设置为C是为了过滤掉字牌
	return ((tile0 & 0xCF) == (tile1 & 0xCF));
}

/**
 * end group
 * @}
 */

}

#endif

/*** End of inlined file: tile.h ***/


/*** Start of inlined file: shanten.h ***/
#ifndef __MAHJONG_ALGORITHM__SHANTEN_H__
#define __MAHJONG_ALGORITHM__SHANTEN_H__

namespace mahjong {

/**
 * @brief 牌组转换成牌
 *
 * @param [in] packs 牌组
 * @param [in] pack_cnt 牌组的数量
 * @param [out] tiles 牌
 * @param [in] tile_cnt 牌的最大数量
 * @return intptr_t 牌的实际数量
 */
intptr_t packs_to_tiles(const pack_t *packs, intptr_t pack_cnt, tile_t *tiles, intptr_t tile_cnt);

/**
 * @brief 将牌打表
 *
 * @param [in] tiles 牌
 * @param [in] cnt 牌的数量
 * @param [out] cnt_table 牌的数量表
 */
void map_tiles(const tile_t *tiles, intptr_t cnt, tile_table_t *cnt_table);

/**
 * @brief 将手牌打表
 *
 * @param [in] hand_tiles 手牌
 * @param [out] cnt_table 牌的数量表
 * @return bool 手牌结构是否正确。即是否符合：副露组数*3+立牌数=13
 */
bool map_hand_tiles(const hand_tiles_t *hand_tiles, tile_table_t *cnt_table);

/**
 * @brief 将表转换成牌
 *
 * @param [in] cnt_table 牌的数量表
 * @param [out] tiles 牌
 * @param [in] max_cnt 牌的最大数量
 * @return intptr_t 牌的实际数量
 */
intptr_t table_to_tiles(const tile_table_t &cnt_table, tile_t *tiles, intptr_t max_cnt);

/**
 * @brief 有效牌标记表类型
 */
typedef bool useful_table_t[TILE_TABLE_SIZE];

/**
 * @addtogroup shanten
 * @{
 */

/**
 * @addtogroup basic_form
 * @{
 */

/**
 * @brief 基本和型上听数
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] useful_table 有效牌标记表（可为null）
 * @return int 上听数
 */
int basic_form_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table);

/**
 * @brief 基本和型是否听牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] waiting_table 听牌标记表（可为null）
 * @return bool 是否听牌
 */
bool is_basic_form_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table);

/**
 * @brief 基本和型是否和牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [in] test_tile 测试的牌
 * @return bool 是否和牌
 */
bool is_basic_form_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile);

/**
 * end group
 * @}
 */

/**
 * @addtogroup seven_pairs
 * @{
 */

/**
 * @brief 七对上听数
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] useful_table 有效牌标记表（可为null）
 * @return int 上听数
 */
int seven_pairs_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table);

/**
 * @brief 七对是否听牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] waiting_table 听牌标记表（可为null）
 * @return bool 是否听牌
 */
bool is_seven_pairs_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table);

/**
 * @brief 七对是否和牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [in] test_tile 测试的牌
 * @return bool 是否和牌
 */
bool is_seven_pairs_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile);

/**
 * end group
 * @}
 */

/**
 * @addtogroup thirteen_orphans
 * @{
 */

/**
 * @brief 十三幺上听数
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] useful_table 有效牌标记表（可为null）
 * @return int 上听数
 */
int thirteen_orphans_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table);

/**
 * @brief 十三幺是否听牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] waiting_table 听牌标记表（可为null）
 * @return bool 是否听牌
 */
bool is_thirteen_orphans_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table);

/**
 * @brief 十三幺是否和牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [in] test_tile 测试的牌
 * @return bool 是否和牌
 */
bool is_thirteen_orphans_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile);

/**
 * end group
 * @}
 */

/**
 * @addtogroup knitted_straight
 * @{
 */

/**
 * @brief 组合龙上听数
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] useful_table 有效牌标记表（可为null）
 * @return int 上听数
 */
int knitted_straight_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table);

/**
 * @brief 组合龙是否听牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] waiting_table 听牌标记表（可为null）
 * @return bool 是否听牌
 */
bool is_knitted_straight_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table);

/**
 * @brief 组合龙是否和牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [in] test_tile 测试的牌
 * @return bool 是否和牌
 */
bool is_knitted_straight_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile);

/**
 * end group
 * @}
 */

/**
 * @addtogroup honors_and_knitted_tiles
 * @{
 */

/**
 * @brief 全不靠上听数
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] useful_table 有效牌标记表（可为null）
 * @return int 上听数
 */
int honors_and_knitted_tiles_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table);

/**
 * @brief 全不靠是否听牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [out] waiting_table 听牌标记表（可为null）
 * @return bool 是否听牌
 */
bool is_honors_and_knitted_tiles_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table);

/**
 * @brief 全不靠是否和牌
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [in] test_tile 测试的牌
 * @return bool 是否和牌
 */
bool is_honors_and_knitted_tiles_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile);

/**
 * end group
 * @}
 */

/**
 * @brief 是否听牌
 *
 * @param [in] hand_tiles 手牌结构
 * @param [out] useful_table 有效牌标记表（可为null）
 * @return bool 是否听牌
 */
bool is_waiting(const hand_tiles_t &hand_tiles, useful_table_t *useful_table);

/**
 * end group
 * @}
 */

/**
 * @name form flags
 * @{
 *  和型
 */
#define FORM_FLAG_BASIC_FORM                0x01  ///< 基本和型
#define FORM_FLAG_SEVEN_PAIRS               0x02  ///< 七对
#define FORM_FLAG_THIRTEEN_ORPHANS          0x04  ///< 十三幺
#define FORM_FLAG_HONORS_AND_KNITTED_TILES  0x08  ///< 全不靠
#define FORM_FLAG_KNITTED_STRAIGHT          0x10  ///< 组合龙
#define FORM_FLAG_ALL                       0xFF  ///< 全部和型
/**
 * @}
 */

/**
 * @brief 枚举打哪张牌的计算结果信息
 */
struct enum_result_t {
	tile_t discard_tile;                    ///< 打这张牌
	uint8_t form_flag;                      ///< 和牌形式
	int shanten;                            ///< 上听数
	useful_table_t useful_table;            ///< 有效牌标记表
};

/**
 * @brief 枚举打哪张牌的计算回调函数
 *
 * @param [in] context 从enum_discard_tile传过来的context原样传回
 * @param [in] result 计算结果
 * @retval true 继续枚举
 * @retval false 结束枚举
 */
typedef bool (*enum_callback_t)(void *context, const enum_result_t *result);

/**
 * @brief 枚举打哪张牌
 *
 * @param [in] hand_tiles 手牌结构
 * @param [in] serving_tile 上牌（可为0，此时仅计算手牌的信息）
 * @param [in] form_flag 计算哪些和型
 * @param [in] context 用户自定义参数，将原样从回调函数传回
 * @param [in] enum_callback 回调函数
 */
void enum_discard_tile(const hand_tiles_t *hand_tiles, tile_t serving_tile, uint8_t form_flag,
	void *context, enum_callback_t enum_callback);

}

/**
 * end group
 * @}
 */

#endif

/*** End of inlined file: shanten.h ***/


/*** Start of inlined file: stringify.h ***/
#ifndef __MAHJONG_ALGORITHM__STRINGIFY_H__
#define __MAHJONG_ALGORITHM__STRINGIFY_H__

namespace mahjong {

/**
 * @brief 字符串格式：
 * - 数牌：万=m 条=s 饼=p。后缀使用小写字母，一连串同花色的数牌可合并使用用一个后缀，如123m、678s等等。
 * - 字牌：东南西北=ESWN，中发白=CFP。使用大写字母。亦兼容天凤风格的后缀z，但按中国习惯顺序567z为中发白。
 * - 吃、碰、杠用英文[]，可选用逗号+数字表示供牌来源。数字的具体规则如下：
 *    - 吃：表示第几张牌是由上家打出，如[567m,2]表示57万吃6万（第2张）。对于不指定数字的，默认为吃第1张。
 *    - 碰：表示由哪家打出，1为上家，2为对家，3为下家，如[999s,3]表示碰下家的9条。对于不指定数字的，默认为碰上家。
 *    - 杠：与碰类似，但对于不指定数字的，则认为是暗杠。例如：[SSSS]表示暗杠南；[8888p,1]表示大明杠上家的8饼。当数字为5、6、7时，表示加杠。例如：[1111s,6]表示碰对家的1条后，又摸到1条加杠。
 * - 范例
 *    - [EEEE][CCCC][FFFF][PPPP]NN
 *    - 1112345678999s9s
 *    - [WWWW,1][444s]45m678pFF6m
 *    - [EEEE]288s349pSCFF2p
 *    - [123p,1][345s,2][999s,3]6m6pEW1m
 *    - 356m18s1579pWNFF9p
 */

/**
 * @addtogroup stringify
 * @{
 */

/**
 * @name error codes
 * @{
 *  解析牌的错误码
 */
#define PARSE_NO_ERROR 0                                ///< 无错误
#define PARSE_ERROR_ILLEGAL_CHARACTER -1                ///< 非法字符
#define PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT -2            ///< 数字后面缺少后缀
#define PARSE_ERROR_WRONG_TILES_COUNT_FOR_FIXED_PACK -3 ///< 副露包含错误的牌数目
#define PARSE_ERROR_CANNOT_MAKE_FIXED_PACK -4           ///< 无法正确解析副露
#define PARSE_ERROR_TOO_MANY_FIXED_PACKS -5             ///< 过多组副露（一副合法手牌最多4副露）
#define PARSE_ERROR_TOO_MANY_TILES -6                   ///< 过多牌
#define PARSE_ERROR_TILE_COUNT_GREATER_THAN_4 -7        ///< 某张牌出现超过4枚

 /**
  * @}
  */

/**
 * @brief 解析牌
 * @param [in] str 字符串
 * @param [out] tiles 牌
 * @param [in] max_cnt 牌的最大数量
 * @retval > 0 实际牌的数量
 * @retval == 0 失败
 */
intptr_t parse_tiles(const char *str, tile_t *tiles, intptr_t max_cnt);

/**
 * @brief 字符串转换为手牌结构和上牌
 * @param [in] str 字符串
 * @param [out] hand_tiles 手牌结构
 * @param [out] serving_tile 上的牌
 * @retval PARSE_NO_ERROR 无错误
 * @retval PARSE_ERROR_ILLEGAL_CHARACTER 非法字符
 * @retval PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT 数字后面缺少后缀
 * @retval PARSE_ERROR_WRONG_TILES_COUNT_FOR_FIXED_PACK 副露包含错误的牌数目
 * @retval PARSE_ERROR_CANNOT_MAKE_FIXED_PACK 无法正确解析副露
 * @retval PARSE_ERROR_TOO_MANY_FIXED_PACKS 过多组副露（一副合法手牌最多4副露）
 * @retval PARSE_ERROR_TOO_MANY_TILES 过多牌
 * @retval PARSE_ERROR_TILE_COUNT_GREATER_THAN_4 某张牌出现超过4枚
 */
intptr_t string_to_tiles(const char *str, hand_tiles_t *hand_tiles, tile_t *serving_tile);

/**
 * @brief 牌转换为字符串
 * @param [in] tiles 牌
 * @param [in] tile_cnt 牌的数量
 * @param [out] str 字符串
 * @param [in] max_size 字符串最大长度
 * @return intptr_t 写入的字符串数
 */
intptr_t tiles_to_string(const tile_t *tiles, intptr_t tile_cnt, char *str, intptr_t max_size);

/**
 * @brief 牌组转换为字符串
 * @param [in] packs 牌组
 * @param [in] pack_cnt 牌组的数量
 * @param [out] str 字符串
 * @param [in] max_size 字符串最大长度
 * @return intptr_t 写入的字符串数
 */
intptr_t packs_to_string(const pack_t *packs, intptr_t pack_cnt, char *str, intptr_t max_size);

/**
 * @brief 手牌结构转换为字符串
 * @param [in] hand_tiles 手牌结构
 * @param [out] str 字符串
 * @param [in] max_size 字符串最大长度
 * @return intptr_t 写入的字符串数
 */
intptr_t hand_tiles_to_string(const hand_tiles_t *hand_tiles, char *str, intptr_t max_size);

/**
 * end group
 * @}
 */

}

#endif

/*** End of inlined file: stringify.h ***/


/*** Start of inlined file: fan_calculator.h ***/
#ifndef __MAHJONG_ALGORITHM__FAN_CALCULATOR_H__
#define __MAHJONG_ALGORITHM__FAN_CALCULATOR_H__

#define SUPPORT_CONCEALED_KONG_AND_MELDED_KONG 1  // 支持明暗杠

namespace mahjong {

/**
 * @addtogroup calculator
 * @{
 */

/**
 * @brief 番种
 */
enum fan_t {
	FAN_NONE = 0,                       ///< 无效
	BIG_FOUR_WINDS,                     ///< 大四喜
	BIG_THREE_DRAGONS,                  ///< 大三元
	ALL_GREEN,                          ///< 绿一色
	NINE_GATES,                         ///< 九莲宝灯
	FOUR_KONGS,                         ///< 四杠
	SEVEN_SHIFTED_PAIRS,                ///< 连七对
	THIRTEEN_ORPHANS,                   ///< 十三幺

	ALL_TERMINALS,                      ///< 清幺九
	LITTLE_FOUR_WINDS,                  ///< 小四喜
	LITTLE_THREE_DRAGONS,               ///< 小三元
	ALL_HONORS,                         ///< 字一色
	FOUR_CONCEALED_PUNGS,               ///< 四暗刻
	PURE_TERMINAL_CHOWS,                ///< 一色双龙会

	QUADRUPLE_CHOW,                     ///< 一色四同顺
	FOUR_PURE_SHIFTED_PUNGS,            ///< 一色四节高

	FOUR_PURE_SHIFTED_CHOWS,            ///< 一色四步高
	THREE_KONGS,                        ///< 三杠
	ALL_TERMINALS_AND_HONORS,           ///< 混幺九

	SEVEN_PAIRS,                        ///< 七对
	GREATER_HONORS_AND_KNITTED_TILES,   ///< 七星不靠
	ALL_EVEN_PUNGS,                     ///< 全双刻
	FULL_FLUSH,                         ///< 清一色
	PURE_TRIPLE_CHOW,                   ///< 一色三同顺
	PURE_SHIFTED_PUNGS,                 ///< 一色三节高
	UPPER_TILES,                        ///< 全大
	MIDDLE_TILES,                       ///< 全中
	LOWER_TILES,                        ///< 全小

	PURE_STRAIGHT,                      ///< 清龙
	THREE_SUITED_TERMINAL_CHOWS,        ///< 三色双龙会
	PURE_SHIFTED_CHOWS,                 ///< 一色三步高
	ALL_FIVE,                           ///< 全带五
	TRIPLE_PUNG,                        ///< 三同刻
	THREE_CONCEALED_PUNGS,              ///< 三暗刻

	LESSER_HONORS_AND_KNITTED_TILES,    ///< 全不靠
	KNITTED_STRAIGHT,                   ///< 组合龙
	UPPER_FOUR,                         ///< 大于五
	LOWER_FOUR,                         ///< 小于五
	BIG_THREE_WINDS,                    ///< 三风刻

	MIXED_STRAIGHT,                     ///< 花龙
	REVERSIBLE_TILES,                   ///< 推不倒
	MIXED_TRIPLE_CHOW,                  ///< 三色三同顺
	MIXED_SHIFTED_PUNGS,                ///< 三色三节高
	CHICKEN_HAND,                       ///< 无番和
	LAST_TILE_DRAW,                     ///< 妙手回春
	LAST_TILE_CLAIM,                    ///< 海底捞月
	OUT_WITH_REPLACEMENT_TILE,          ///< 杠上开花
	ROBBING_THE_KONG,                   ///< 抢杠和

	ALL_PUNGS,                          ///< 碰碰和
	HALF_FLUSH,                         ///< 混一色
	MIXED_SHIFTED_CHOWS,                ///< 三色三步高
	ALL_TYPES,                          ///< 五门齐
	MELDED_HAND,                        ///< 全求人
	TWO_CONCEALED_KONGS,                ///< 双暗杠
	TWO_DRAGONS_PUNGS,                  ///< 双箭刻

	OUTSIDE_HAND,                       ///< 全带幺
	FULLY_CONCEALED_HAND,               ///< 不求人
	TWO_MELDED_KONGS,                   ///< 双明杠
	LAST_TILE,                          ///< 和绝张

	DRAGON_PUNG,                        ///< 箭刻
	PREVALENT_WIND,                     ///< 圈风刻
	SEAT_WIND,                          ///< 门风刻
	CONCEALED_HAND,                     ///< 门前清
	ALL_CHOWS,                          ///< 平和
	TILE_HOG,                           ///< 四归一
	DOUBLE_PUNG,                        ///< 双同刻
	TWO_CONCEALED_PUNGS,                ///< 双暗刻
	CONCEALED_KONG,                     ///< 暗杠
	ALL_SIMPLES,                        ///< 断幺

	PURE_DOUBLE_CHOW,                   ///< 一般高
	MIXED_DOUBLE_CHOW,                  ///< 喜相逢
	SHORT_STRAIGHT,                     ///< 连六
	TWO_TERMINAL_CHOWS,                 ///< 老少副
	PUNG_OF_TERMINALS_OR_HONORS,        ///< 幺九刻
	MELDED_KONG,                        ///< 明杠
	ONE_VOIDED_SUIT,                    ///< 缺一门
	NO_HONORS,                          ///< 无字
	EDGE_WAIT,                          ///< 边张
	CLOSED_WAIT,                        ///< 嵌张
	SINGLE_WAIT,                        ///< 单钓将
	SELF_DRAWN,                         ///< 自摸

	FLOWER_TILES,                       ///< 花牌

#if SUPPORT_CONCEALED_KONG_AND_MELDED_KONG
	CONCEALED_KONG_AND_MELDED_KONG,     ///< 明暗杠
#endif

	FAN_TABLE_SIZE
};

/**
 * @brief 风（用来表示圈风门风）
 */
enum class wind_t {
	EAST, SOUTH, WEST, NORTH
};

/**
 * @brief 和牌标记
 */
typedef uint8_t win_flag_t;

/**
 * @name win flag
 * @{
 */
#define WIN_FLAG_DISCARD    0   ///< 点和
#define WIN_FLAG_SELF_DRAWN 1   ///< 自摸
#define WIN_FLAG_4TH_TILE   2   ///< 绝张
#define WIN_FLAG_ABOUT_KONG 4   ///< 关于杠，复合点和时为枪杠和，复合自摸则为杠上开花
#define WIN_FLAG_WALL_LAST  8   ///< 牌墙最后一张，复合点和时为海底捞月，复合自摸则为妙手回春
#define WIN_FLAG_INIT       16  ///< 起手，复合点和时为地和，复合自摸则为天和

/**
 * @}
 */

/**
 * @name error codes
 * @{
 */
#define ERROR_WRONG_TILES_COUNT -1              ///< 错误的张数
#define ERROR_TILE_COUNT_GREATER_THAN_4 -2      ///< 某张牌出现超过4枚
#define ERROR_NOT_WIN -3                        ///< 没和牌
/**
 * @}
 */

/**
 * @brief 检查算番的输入是否合法
 *
 * @param [in] hand_tiles 手牌
 * @param [in] win_tile 和牌张
 * @retval 0 成功
 * @retval ERROR_WRONG_TILES_COUNT 错误的张数
 * @retval ERROR_TILE_COUNT_GREATER_THAN_4 某张牌出现超过4枚
 */
int check_calculator_input(const hand_tiles_t *hand_tiles, tile_t win_tile);

/**
 * @brief 算番参数
 */
struct calculate_param_t {
	hand_tiles_t hand_tiles;    ///< 手牌
	tile_t win_tile;            ///< 和牌张
	uint8_t flower_count;       ///< 花牌数
	win_flag_t win_flag;        ///< 和牌标记
	wind_t prevalent_wind;      ///< 圈风
	wind_t seat_wind;           ///< 门风
};

/**
 * @brief 番表
 */
typedef uint16_t fan_table_t[FAN_TABLE_SIZE];

/**
 * @brief 算番
 *
 * @param [in] calculate_param 算番参数
 * @param [out] fan_table 番表，当有某种番时，相应的会设置为这种番出现的次数
 * @retval >0 番数
 * @retval ERROR_WRONG_TILES_COUNT 错误的张数
 * @retval ERROR_TILE_COUNT_GREATER_THAN_4 某张牌出现超过4枚
 * @retval ERROR_NOT_WIN 没和牌
 */
int calculate_fan(const calculate_param_t *calculate_param, fan_table_t *fan_table);

#if 0

/**
 * @brief 番名（英文）
 */
static const char *fan_name[] = {
	"None",
	"Big Four Winds", "Big Three Dragons", "All Green", "Nine Gates", "Four Kongs", "Seven Shifted Pairs", "Thirteen Orphans",
	"All Terminals", "Little Four Winds", "Little Three Dragons", "All Honors", "Four Concealed Pungs", "Pure Terminal Chows",
	"Quadruple Chow", "Four Pure Shifted Pungs",
	"Four Pure Shifted Chows", "Three Kongs", "All Terminals and Honors",
	"Seven Pairs", "Greater Honors and Knitted Tiles", "All Even Pungs", "Full Flush", "Pure Triple Chow", "Pure Shifted Pungs", "Upper Tiles", "Middle Tiles", "Lower Tiles",
	"Pure Straight", "Three-Suited Terminal Chows", "Pure Shifted Chows", "All Five", "Triple Pung", "Three Concealed Pungs",
	"Lesser Honors and Knitted Tiles", "Knitted Straight", "Upper Four", "Lower Four", "Big Three Winds",
	"Mixed Straight", "Reversible Tiles", "Mixed Triple Chow", "Mixed Shifted Pungs", "Chicken Hand", "Last Tile Draw", "Last Tile Claim", "Out with Replacement Tile", "Robbing The Kong",
	"All Pungs", "Half Flush", "Mixed Shifted Chows", "All Types", "Melded Hand", "Two Concealed Kongs", "Two Dragons Pungs",
	"Outside Hand", "Fully Concealed Hand", "Two Melded Kongs", "Last Tile",
	"Dragon Pung", "Prevalent Wind", "Seat Wind", "Concealed Hand", "All Chows", "Tile Hog", "Double Pung",
	"Two Concealed Pungs", "Concealed Kong", "All Simples",
	"Pure Double Chow", "Mixed Double Chow", "Short Straight", "Two Terminal Chows", "Pung of Terminals or Honors", "Melded Kong", "One Voided Suit", "No Honors", "Edge Wait", "Closed Wait", "Single Wait", "Self-Drawn",
	"Flower Tiles"
#if SUPPORT_CONCEALED_KONG_AND_MELDED_KONG
	, "Concealed Kong and Melded Kong"
#endif
};

#else

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

// u8 literal prefix
#ifndef __UTF8_TEXT
// VS2015 GCC4.7 Clang5.0
#if (defined(_MSC_VER) && (_MSC_VER >= 1900)) || (defined(__GNUC__) && ((__GNUC__ << 8 | __GNUC_MINOR__) >= 0x407)) || (defined(__clang__) && (__clang_major__ >= 5))
#define __UTF8_TEXT(quote) u8 ## quote
#else
#define __UTF8_TEXT(quote) quote
#endif
#endif

#ifndef __UTF8
#define __UTF8(quote) __UTF8_TEXT(quote)
#endif

/**
 * @brief 番名（简体中文）
 */
static const char *fan_name[] = {
	__UTF8("无"),
	__UTF8("大四喜"), __UTF8("大三元"), __UTF8("绿一色"), __UTF8("九莲宝灯"), __UTF8("四杠"), __UTF8("连七对"), __UTF8("十三幺"),
	__UTF8("清幺九"), __UTF8("小四喜"), __UTF8("小三元"), __UTF8("字一色"), __UTF8("四暗刻"), __UTF8("一色双龙会"),
	__UTF8("一色四同顺"), __UTF8("一色四节高"),
	__UTF8("一色四步高"), __UTF8("三杠"), __UTF8("混幺九"),
	__UTF8("七对"), __UTF8("七星不靠"), __UTF8("全双刻"), __UTF8("清一色"), __UTF8("一色三同顺"), __UTF8("一色三节高"), __UTF8("全大"), __UTF8("全中"), __UTF8("全小"),
	__UTF8("清龙"), __UTF8("三色双龙会"), __UTF8("一色三步高"), __UTF8("全带五"), __UTF8("三同刻"), __UTF8("三暗刻"),
	__UTF8("全不靠"), __UTF8("组合龙"), __UTF8("大于五"), __UTF8("小于五"), __UTF8("三风刻"),
	__UTF8("花龙"), __UTF8("推不倒"), __UTF8("三色三同顺"), __UTF8("三色三节高"), __UTF8("无番和"), __UTF8("妙手回春"), __UTF8("海底捞月"), __UTF8("杠上开花"), __UTF8("抢杠和"),
	__UTF8("碰碰和"), __UTF8("混一色"), __UTF8("三色三步高"), __UTF8("五门齐"), __UTF8("全求人"), __UTF8("双暗杠"), __UTF8("双箭刻"),
	__UTF8("全带幺"), __UTF8("不求人"), __UTF8("双明杠"), __UTF8("和绝张"),
	__UTF8("箭刻"), __UTF8("圈风刻"), __UTF8("门风刻"), __UTF8("门前清"), __UTF8("平和"), __UTF8("四归一"), __UTF8("双同刻"), __UTF8("双暗刻"), __UTF8("暗杠"), __UTF8("断幺"),
	__UTF8("一般高"), __UTF8("喜相逢"), __UTF8("连六"), __UTF8("老少副"), __UTF8("幺九刻"), __UTF8("明杠"), __UTF8("缺一门"), __UTF8("无字"), __UTF8("边张"), __UTF8("嵌张"), __UTF8("单钓将"), __UTF8("自摸"),
	__UTF8("花牌")
#if SUPPORT_CONCEALED_KONG_AND_MELDED_KONG
	, __UTF8("明暗杠")
#endif
};

#endif

/**
 * @brief 番值
 */
static const uint16_t fan_value_table[FAN_TABLE_SIZE] = {
	0,
	88, 88, 88, 88, 88, 88, 88,
	64, 64, 64, 64, 64, 64,
	48, 48,
	32, 32, 32,
	24, 24, 24, 24, 24, 24, 24, 24, 24,
	16, 16, 16, 16, 16, 16,
	12, 12, 12, 12, 12,
	8, 8, 8, 8, 8, 8, 8, 8, 8,
	6, 6, 6, 6, 6, 6, 6,
	4, 4, 4, 4,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1
#if SUPPORT_CONCEALED_KONG_AND_MELDED_KONG
	, 5
#endif
};

/**
 * @brief 判断立牌是否包含和牌
 * 如果是，则必然不是和绝张
 *
 * @param [in] standing_tiles 立牌
 * @param [in] standing_cnt 立牌数
 * @param [in] win_tile 和牌张
 * @return bool
 */
bool is_standing_tiles_contains_win_tile(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t win_tile);

/**
 * @brief 统计和牌在副露牌组中出现的张数
 * 如果出现3张，则必然和绝张
 *
 * @param [in] fixed_packs 副露牌组
 * @param [in] fixed_cnt 副露牌组数
 * @param [in] win_tile 和牌张
 * @return size_t
 */
size_t count_win_tile_in_fixed_packs(const pack_t *fixed_packs, intptr_t fixed_cnt, tile_t win_tile);

/**
 * @brief 判断副露牌组是否包含杠
 *
 * @param [in] fixed_packs 副露牌组
 * @param [in] fixed_cnt 副露牌组数
 * @return bool
 */
bool is_fixed_packs_contains_kong(const pack_t *fixed_packs, intptr_t fixed_cnt);

/**
 * end group
 * @}
 */

}

#endif

/*** End of inlined file: fan_calculator.h ***/


/*** Start of inlined file: MahjongGB.h ***/
#ifndef MAHJONG_H
#define MAHJONG_H

#include <utility>
#include <vector>
#include <string>

//CPP

/*** Start of inlined file: MahjongGB.cpp ***/
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>

#include <cstring>
#include <iostream>

using namespace std;

static unordered_map<string, mahjong::tile_t> str2tile;

vector<pair<int, string> > MahjongFanCalculator(
	vector<pair<string, pair<string, int> > > pack,
	vector<string> hand,
	string winTile,
	int flowerCount,
	bool isZIMO,
	bool isJUEZHANG,
	bool isGANG,
	bool isLAST,
	int menFeng,
	int quanFeng)
{
	vector<pair<int,string>> ans;
	mahjong::calculate_param_t calculate_param;
	mahjong::fan_table_t fan_table;
	memset(&calculate_param, 0, sizeof(mahjong::calculate_param_t));
	memset(&fan_table, 0, sizeof(mahjong::fan_table_t));
	calculate_param.hand_tiles.tile_count = hand.size();
	for(unsigned int i = 0; i < hand.size(); i++) {
		if(str2tile.find(hand[i]) == str2tile.end()){
			throw string("ERROE_WRONG_TILE_CODE");
		}
		calculate_param.hand_tiles.standing_tiles[i] = str2tile[hand[i]];
	}
	calculate_param.hand_tiles.pack_count = pack.size();
	for(unsigned int i = 0; i < pack.size(); i++) {
		pair<string, pair<string, int>> &sPack = pack[i];
		mahjong::pack_t &dPack = calculate_param.hand_tiles.fixed_packs[i];
		if(sPack.first == "PENG") {
			dPack = mahjong::make_pack(sPack.second.second, PACK_TYPE_PUNG, str2tile[sPack.second.first]);
		} else if(sPack.first == "GANG") {
			dPack = mahjong::make_pack(sPack.second.second, PACK_TYPE_KONG, str2tile[sPack.second.first]);
		} else if(sPack.first == "CHI"){
			dPack = mahjong::make_pack(sPack.second.second, PACK_TYPE_CHOW, str2tile[sPack.second.first]);
		} else {
			throw string("ERROE_WRONG_PACK_CODE");
		}
	}
	calculate_param.win_tile = str2tile[winTile];
	calculate_param.flower_count = flowerCount;
	if(isZIMO) {
		calculate_param.win_flag |= WIN_FLAG_SELF_DRAWN;
	}
	if(isLAST) {
		calculate_param.win_flag |= WIN_FLAG_WALL_LAST;
	}
	if(isJUEZHANG) {
		calculate_param.win_flag |= WIN_FLAG_4TH_TILE;
	}
	if(isGANG) {
		calculate_param.win_flag |= WIN_FLAG_ABOUT_KONG;
	}
	calculate_param.prevalent_wind = (mahjong::wind_t)quanFeng;
	calculate_param.seat_wind = (mahjong::wind_t)menFeng;
	int re = mahjong::calculate_fan(&calculate_param, &fan_table);
	if(re == -1) {
		throw string("ERROR_WRONG_TILES_COUNT");
	}else if(re == -2) {
		throw string("ERROR_TILE_COUNT_GREATER_THAN_4");
	}else if(re == -3) {
		throw string("ERROR_NOT_WIN");
	}
	for(int i = 0; i < mahjong::FAN_TABLE_SIZE; i++) {
		if(fan_table[i] > 0) {
			ans.push_back(make_pair(fan_table[i]*mahjong::fan_value_table[i],mahjong::fan_name[i]));
		}
	}
	return ans;
}

void MahjongInit()
{
	for(int i = 1; i <= 9; i++) {
		str2tile["W" + to_string(i)] = mahjong::make_tile(TILE_SUIT_CHARACTERS, i);
		str2tile["B" + to_string(i)] = mahjong::make_tile(TILE_SUIT_DOTS, i);
		str2tile["T" + to_string(i)] = mahjong::make_tile(TILE_SUIT_BAMBOO, i);
	}
	for(int i = 1; i <= 4; i++) {
		str2tile["F" + to_string((i))] = mahjong::make_tile(TILE_SUIT_HONORS, i);
	}
	for(int i = 1; i <= 3; i++) {
		str2tile["J" + to_string((i))] = mahjong::make_tile(TILE_SUIT_HONORS, i + 4);
	}
}
/*** End of inlined file: MahjongGB.cpp ***/



/*** Start of inlined file: fan_calculator.cpp ***/
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <algorithm>
#include <iterator>

/*** Start of inlined file: standard_tiles.h ***/
#ifndef __MAHJONG_ALGORITHM__STANDARD_TILES_H__
#define __MAHJONG_ALGORITHM__STANDARD_TILES_H__

namespace mahjong {

// 十三幺13面听
static const tile_t standard_thirteen_orphans[13] = {
	TILE_1m, TILE_9m, TILE_1s, TILE_9s, TILE_1p, TILE_9p, TILE_E, TILE_S, TILE_W, TILE_N, TILE_C, TILE_F, TILE_P
};

// 组合龙只有如下6种
// 147m 258s 369p
// 147m 369s 258p
// 258m 147s 369p
// 258m 369s 147p
// 369m 147s 258p
// 369m 258s 147p
static const tile_t standard_knitted_straight[6][9] = {
	{ TILE_1m, TILE_4m, TILE_7m, TILE_2s, TILE_5s, TILE_8s, TILE_3p, TILE_6p, TILE_9p },
	{ TILE_1m, TILE_4m, TILE_7m, TILE_3s, TILE_6s, TILE_9s, TILE_2p, TILE_5p, TILE_8p },
	{ TILE_2m, TILE_5m, TILE_8m, TILE_1s, TILE_4s, TILE_7s, TILE_3p, TILE_6p, TILE_9p },
	{ TILE_2m, TILE_5m, TILE_8m, TILE_3s, TILE_6s, TILE_9s, TILE_1p, TILE_4p, TILE_7p },
	{ TILE_3m, TILE_6m, TILE_9m, TILE_1s, TILE_4s, TILE_7s, TILE_2p, TILE_5p, TILE_8p },
	{ TILE_3m, TILE_6m, TILE_9m, TILE_2s, TILE_5s, TILE_8s, TILE_1p, TILE_4p, TILE_7p },
};

}

#endif

/*** End of inlined file: standard_tiles.h ***/


/**
 * 算番流程概述：
 * 1. 判断特殊和型
 *   (1) 门清状态——七对、十三幺、组合龙、全不靠
 *   (2) 1副露状态——组合龙
 * 2. 按基本和型划分（留意七对）
 * 3. 对第2步中的划分结果进行算番，取最大值

 * 对划分后的结果算番流程：
 * 1. 计数顺子个数和刻子个数，对刻子自身算番（涉及：幺九刻、箭刻）
 * 2. 分情况对面子算番
 *   (1) 4顺——先判断三色/一色双龙会，没有再计算4顺的番
 *   (2) 3顺1刻——计算3顺的番
 *   (3) 2顺2刻——计算2顺的番，计算2刻的番
 *   (4) 1顺3刻——计算3刻的番
 * 3. 九莲宝灯判断
 * 4. 根据和牌方式调整——涉及：不求人、全求人
 * 5. 根据雀头调整——涉及：平和、小三元、小四喜
 * 6. 根据牌组特征调整——涉及：全带幺、全带五、全双刻
 * 7. 根据花色调整——涉及：无字、缺一门、混一色、清一色、五门齐
 * 8. 根据牌特性调整——涉及：断幺、推不倒、绿一色、字一色、清幺九、混幺九
 * 9. 根据数牌的范围调整——涉及：大于五、小于五、全大、全中、全小
 * 10. 计算四归一
 * 11. 根据听牌方式调整——涉及：边张、嵌张、单钓将
 * 12. 统一调整规则中规定不计的
 * 13. 最后调整圈风门风
 * 14. 以上流程走完，得到算番结果。如果为0番，则调整为无番和
 */

#define MAX_DIVISION_CNT 20  // 一副牌最多也没有20种划分吧，够用了

#if 0
#define LOG(fmt_, ...) printf(fmt_, ##__VA_ARGS__)
#else
#define LOG(...) ((void)0)
#endif

//#define STRICT_98_RULE

namespace mahjong {

#if 0  // Debug
extern intptr_t packs_to_string(const pack_t *packs, intptr_t pack_cnt, char *str, intptr_t max_size);
#endif

//-------------------------------- 划分 --------------------------------

namespace {

	// 划分
	struct division_t {
		pack_t packs[5];  // 牌组。4面子1雀头，共5组
	};

	// 划分结果
	struct division_result_t {
		division_t divisions[MAX_DIVISION_CNT];  // 每一种划分
		intptr_t count;  // 划分方式总数
	};
}

// 递归划分算法的最后一步，添加划分
static void divide_tail_add_division(intptr_t fixed_cnt, const division_t *work_division, division_result_t *result) {
	// 拷贝一份当前的划分出来的面子，并排序暗手的面子
	// 这里不能直接在work_division->packs上排序，否则会破坏递归外层的数据
	division_t temp;
	memcpy(&temp, work_division, sizeof(temp));
	std::sort(temp.packs + fixed_cnt, temp.packs + 4);

	// 如果这种划分是否已经存在了
	if (std::none_of(&result->divisions[0], &result->divisions[result->count],
		[&temp, fixed_cnt](const division_t &od) {
		return std::equal(&od.packs[fixed_cnt], &od.packs[4], &temp.packs[fixed_cnt]);
	})) {
		// 写入划分结果里
		memcpy(&result->divisions[result->count], &temp, sizeof(temp));
		++result->count;
	}
	else {
		LOG("same case");
	}
}

// 递归划分的最后一步
static bool divide_tail(tile_table_t &cnt_table, intptr_t fixed_cnt, division_t *work_division, division_result_t *result) {
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] < 2) {
			continue;
		}

		cnt_table[t] -= 2;  // 削减
		// 所有牌全部使用完毕
		if (std::all_of(std::begin(cnt_table), std::end(cnt_table), [](int n) { return n == 0; })) {
			cnt_table[t] += 2;  // 还原

			// 这2张作为雀头
			work_division->packs[4] = make_pack(0, PACK_TYPE_PAIR, t);
			divide_tail_add_division(fixed_cnt, work_division, result);  // 记录
			return true;
		}
		cnt_table[t] += 2;  // 还原
	}

	return false;
}

// 判断一条划分分支是否来过
static bool is_division_branch_exist(intptr_t fixed_cnt, intptr_t step, const division_t *work_division, const division_result_t *result) {
	// 没有划分时，以及划分步骤小于3时，不检测，因为至少要有3步递归才会产生不同划分
	if (result->count <= 0 || step < 3) {
		return false;
	}

	// std::includes要求有序
	// 这里不能直接在work_division->packs上排序，否则会破坏递归外层的数据
	division_t temp;
	memcpy(&temp.packs[fixed_cnt], &work_division->packs[fixed_cnt], step * sizeof(pack_t));
	std::sort(&temp.packs[fixed_cnt], &temp.packs[fixed_cnt + step]);

	// 只需要比较面子是否重复分支，雀头不参与比较，所以下标是4
	return std::any_of(&result->divisions[0], &result->divisions[result->count],
		[&temp, fixed_cnt, step](const division_t &od) {
		return std::includes(&od.packs[fixed_cnt], &od.packs[4], &temp.packs[fixed_cnt], &temp.packs[fixed_cnt + step]);
	});
}

// 递归划分
static bool divide_recursively(tile_table_t &cnt_table, intptr_t fixed_cnt, intptr_t step, division_t *work_division, division_result_t *result) {
	const intptr_t idx = step + fixed_cnt;
	if (idx == 4) {  // 4组面子都有了
		return divide_tail(cnt_table, fixed_cnt, work_division, result);
	}

	bool ret = false;

	// 按牌表张遍历牌
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] < 1) {
			continue;
		}

		// 刻子
		if (cnt_table[t] > 2) {
			work_division->packs[idx] = make_pack(0, PACK_TYPE_PUNG, t);  // 记录刻子
			if (!is_division_branch_exist(fixed_cnt, step + 1, work_division, result)) {
				// 削减这组刻子，递归
				cnt_table[t] -= 3;
				if (divide_recursively(cnt_table, fixed_cnt, step + 1, work_division, result)) {
					ret = true;
				}
				// 还原
				cnt_table[t] += 3;
			}
		}

		// 顺子（只能是数牌）
		if (is_numbered_suit(t)) {
			if (tile_get_rank(t) < 8 && cnt_table[t + 1] && cnt_table[t + 2]) {
				work_division->packs[idx] = make_pack(0, PACK_TYPE_CHOW, static_cast<tile_t>(t + 1));  // 记录顺子
				if (!is_division_branch_exist(fixed_cnt, step + 1, work_division, result)) {
					// 削减这组顺子，递归
					--cnt_table[t];
					--cnt_table[t + 1];
					--cnt_table[t + 2];
					if (divide_recursively(cnt_table, fixed_cnt, step + 1, work_division, result)) {
						ret = true;
					}
					// 还原
					++cnt_table[t];
					++cnt_table[t + 1];
					++cnt_table[t + 2];
				}
			}
		}
	}

	return ret;
}

// 划分一手牌
static bool divide_win_hand(const tile_t *standing_tiles, const pack_t *fixed_packs, intptr_t fixed_cnt, division_result_t *result) {
	intptr_t standing_cnt = 14 - fixed_cnt * 3;

	// 对立牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);

	result->count = 0;

	// 复制副露的面子
	division_t work_division;
	memcpy(work_division.packs, fixed_packs, fixed_cnt * sizeof(pack_t));
	return divide_recursively(cnt_table, fixed_cnt, 0, &work_division, result);
}

//-------------------------------- 算番 --------------------------------

// 4组递增1
static FORCE_INLINE bool is_four_shifted_1(rank_t r0, rank_t r1, rank_t r2, rank_t r3) {
	return (r0 + 1 == r1 && r1 + 1 == r2 && r2 + 1 == r3);
}

// 4组递增2
static FORCE_INLINE bool is_four_shifted_2(rank_t r0, rank_t r1, rank_t r2, rank_t r3) {
	return (r0 + 2 == r1 && r1 + 2 == r2 && r2 + 2 == r3);
}

// 3组递增1
static FORCE_INLINE bool is_shifted_1(rank_t r0, rank_t r1, rank_t r2) {
	return (r0 + 1 == r1 && r1 + 1 == r2);
}

// 3组递增2
static FORCE_INLINE bool is_shifted_2(rank_t r0, rank_t r1, rank_t r2) {
	return (r0 + 2 == r1 && r1 + 2 == r2);
}

// 三色
static FORCE_INLINE bool is_mixed(suit_t s0, suit_t s1, suit_t s2) {
	return (s0 != s1 && s0 != s2 && s1 != s2);
}

// 3组递增1无序
static FORCE_INLINE bool is_shifted_1_unordered(rank_t r0, rank_t r1, rank_t r2) {
	return is_shifted_1(r1, r0, r2) || is_shifted_1(r2, r0, r1) || is_shifted_1(r0, r1, r2)
		|| is_shifted_1(r2, r1, r0) || is_shifted_1(r0, r2, r1) || is_shifted_1(r1, r2, r0);
}

// 4组顺子的番
static fan_t get_4_chows_fan(tile_t t0, tile_t t1, tile_t t2, tile_t t3) {
	// 按出现频率顺序

	// 一色四步高
	if (is_four_shifted_2(t0, t1, t2, t3) || is_four_shifted_1(t0, t1, t2, t3)) {
		return FOUR_PURE_SHIFTED_CHOWS;
	}
	// 一色四同顺
	if (t0 == t1 && t0 == t2 && t0 == t3) {
		return QUADRUPLE_CHOW;
	}
	// 以上都没有
	return FAN_NONE;
}

// 3组顺子的番
static fan_t get_3_chows_fan(tile_t t0, tile_t t1, tile_t t2) {
	suit_t s0 = tile_get_suit(t0);
	suit_t s1 = tile_get_suit(t1);
	suit_t s2 = tile_get_suit(t2);

	rank_t r0 = tile_get_rank(t0);
	rank_t r1 = tile_get_rank(t1);
	rank_t r2 = tile_get_rank(t2);

	// 按出现频率顺序

	if (is_mixed(s0, s1, s2)) {  // 三色
		// 三色三步高
		if (is_shifted_1_unordered(r1, r0, r2)) {
			return MIXED_SHIFTED_CHOWS;
		}
		// 三色三同顺
		if (r0 == r1 && r1 == r2) {
			return MIXED_TRIPLE_CHOW;
		}
		// 花龙
		if ((r0 == 2 && r1 == 5 && r2 == 8) || (r0 == 2 && r1 == 8 && r2 == 5)
			|| (r0 == 5 && r1 == 2 && r2 == 8) || (r0 == 5 && r1 == 8 && r2 == 2)
			|| (r0 == 8 && r1 == 2 && r2 == 5) || (r0 == 8 && r1 == 5 && r2 == 2)) {
			return MIXED_STRAIGHT;
		}
	}
	else {  // 一色
		// 清龙
		if (t0 + 3 == t1 && t1 + 3 == t2) {
			return PURE_STRAIGHT;
		}
		// 一色三步高
		if (is_shifted_2(t0, t1, t2) || is_shifted_1(t0, t1, t2)) {
			return PURE_SHIFTED_CHOWS;
		}
		// 一色三同顺
		if (t0 == t1 && t0 == t2) {
			return PURE_TRIPLE_CHOW;
		}
	}
	// 以上都没有
	return FAN_NONE;
}

// 2组顺子的番
static fan_t get_2_chows_fan_unordered(tile_t t0, tile_t t1) {
	// 按出现频率顺序

	if (!is_suit_equal_quick(t0, t1)) {  // 两色
		// 喜相逢
		if (is_rank_equal_quick(t0, t1)) {
			return MIXED_DOUBLE_CHOW;
		}
	}
	else {  // 一色
		// 连六
		if (t0 + 3 == t1 || t1 + 3 == t0) {
			return SHORT_STRAIGHT;
		}
		// 老少副
		rank_t r0 = tile_get_rank(t0);
		rank_t r1 = tile_get_rank(t1);
		if ((r0 == 2 && r1 == 8) || (r0 == 8 && r1 == 2)) {
			return TWO_TERMINAL_CHOWS;
		}
		// 一般高
		if (t0 == t1) {
			return PURE_DOUBLE_CHOW;
		}
	}
	// 以上都没有
	return FAN_NONE;
}

// 4组刻子的番
static fan_t get_4_pungs_fan(tile_t t0, tile_t t1, tile_t t2, tile_t t3) {
	// 一色四节高
	if (is_numbered_suit_quick(t0) && t0 + 1 == t1 && t1 + 1 == t2 && t2 + 1 == t3) {
		return FOUR_PURE_SHIFTED_PUNGS;
	}
	// 大四喜
	if (t0 == TILE_E && t1 == TILE_S && t2 == TILE_W && t3 == TILE_N) {
		return BIG_FOUR_WINDS;
	}
	// 以上都没有
	return FAN_NONE;
}

// 3组刻子的番
static fan_t get_3_pungs_fan(tile_t t0, tile_t t1, tile_t t2) {
	// 按出现频率顺序

	if (is_numbered_suit_quick(t0) && is_numbered_suit_quick(t1) && is_numbered_suit_quick(t2)) {  // 数牌
		suit_t s0 = tile_get_suit(t0);
		suit_t s1 = tile_get_suit(t1);
		suit_t s2 = tile_get_suit(t2);

		rank_t r0 = tile_get_rank(t0);
		rank_t r1 = tile_get_rank(t1);
		rank_t r2 = tile_get_rank(t2);

		if (is_mixed(s0, s1, s2)) {  // 三色
			// 三色三节高
			if (is_shifted_1_unordered(r1, r0, r2)) {
				return MIXED_SHIFTED_PUNGS;
			}
			// 三同刻
			if (r0 == r1 && r1 == r2) {
				return TRIPLE_PUNG;
			}
		}
		else {
			// 一色三节高
			if (t0 + 1 == t1 && t1 + 1 == t2) {
				return PURE_SHIFTED_PUNGS;
			}
		}
	}
	else {
		// 三风刻
		if ((t0 == TILE_E && t1 == TILE_S && t2 == TILE_W)
			|| (t0 == TILE_E && t1 == TILE_S && t2 == TILE_N)
			|| (t0 == TILE_E && t1 == TILE_W && t2 == TILE_N)
			|| (t0 == TILE_S && t1 == TILE_W && t2 == TILE_N)) {
			return BIG_THREE_WINDS;
		}
		// 大三元
		if (t0 == TILE_C && t1 == TILE_F && t2 == TILE_P) {
			return BIG_THREE_DRAGONS;
		}
	}

	// 以上都没有
	return FAN_NONE;
}

// 2组刻子的番
static fan_t get_2_pungs_fan_unordered(tile_t t0, tile_t t1) {
	// 按出现频率顺序
	if (is_numbered_suit_quick(t0) && is_numbered_suit_quick(t1)) {  // 数牌
		// 双同刻
		if (is_rank_equal_quick(t0, t1)) {
			return DOUBLE_PUNG;
		}
	}
	else {
		// 双箭刻
		if (is_dragons(t0) && is_dragons(t1)) {
			return TWO_DRAGONS_PUNGS;
		}
	}
	// 以上都没有
	return FAN_NONE;
}

// 1组刻子的番
static fan_t get_1_pung_fan(tile_t mid_tile) {
	// 箭刻
	if (is_dragons(mid_tile)) {
		return DRAGON_PUNG;
	}
	// 幺九刻
	if (is_terminal(mid_tile) || is_winds(mid_tile)) {
		return PUNG_OF_TERMINALS_OR_HONORS;
	}
	// 以上都没有
	return FAN_NONE;
}

// 存在3组顺子的番种时，余下的第4组顺子最多算1番
static fan_t get_1_chow_extra_fan(tile_t tile0, tile_t tile1, tile_t tile2, tile_t tile_extra) {
	fan_t fan0 = get_2_chows_fan_unordered(tile0, tile_extra);
	fan_t fan1 = get_2_chows_fan_unordered(tile1, tile_extra);
	fan_t fan2 = get_2_chows_fan_unordered(tile2, tile_extra);

	// 按以下顺序返回
	// 一般高
	if (fan0 == PURE_DOUBLE_CHOW || fan1 == PURE_DOUBLE_CHOW || fan2 == PURE_DOUBLE_CHOW) {
		return PURE_DOUBLE_CHOW;
	}
	// 喜相逢
	if (fan0 == MIXED_DOUBLE_CHOW || fan1 == MIXED_DOUBLE_CHOW || fan2 == MIXED_DOUBLE_CHOW) {
		return MIXED_DOUBLE_CHOW;
	}
	// 连六
	if (fan0 == SHORT_STRAIGHT || fan1 == SHORT_STRAIGHT || fan2 == SHORT_STRAIGHT) {
		return SHORT_STRAIGHT;
	}
	// 老少副
	if (fan0 == TWO_TERMINAL_CHOWS || fan1 == TWO_TERMINAL_CHOWS || fan2 == TWO_TERMINAL_CHOWS) {
		return TWO_TERMINAL_CHOWS;
	}

	return FAN_NONE;
}

// 套算一次原则：
// 如果有尚未组合过的一副牌，只可同已组合过的相应的一副牌套算一次
//
// 不得相同原则：
// 凡已经合过某一番种的牌，不能再同其他一副牌组成相同的番种计分
//
// 根据套算一次原则，234567s234567p，只能计为“喜相逢*2 连六*1”或者“喜相逢*1 连六*2”，而不是“喜相逢*2 连六*2”
// 根据以上两点，234s223344567p，只能计为：“一般高、喜相逢、连六”，而不是“喜相逢*2、连六”
//
// 直接按规则来写，差不多是图的算法，太过复杂
// 这里简便处理：先统计有多少番，当超过时规则允许的数目时，从重复的开始削减
static void exclusionary_rule(const fan_t *all_fans, long fan_cnt, long max_cnt, fan_table_t &fan_table) {
	// 统计有多少番
	uint16_t table[4] = { 0 };
	long cnt = 0;
	for (long i = 0; i < fan_cnt; ++i) {
		if (all_fans[i] != FAN_NONE) {
			++cnt;
			++table[all_fans[i] - PURE_DOUBLE_CHOW];
		}
	}

	// 当超过时，从重复的开始削减
	int limit_cnt = 1;
	// 第一轮先削减剩下1，第二轮削减剩下0
	while (cnt > max_cnt && limit_cnt >= 0) {
		int idx = 4;  // 从老少副开始削减
		while (cnt > max_cnt && idx-- > 0) {
			while (static_cast<int>(table[idx]) > limit_cnt && cnt > max_cnt) {
				--table[idx];
				--cnt;
			}
		}
		--limit_cnt;
	}

	fan_table[PURE_DOUBLE_CHOW] = table[0];
	fan_table[MIXED_DOUBLE_CHOW] = table[1];
	fan_table[SHORT_STRAIGHT] = table[2];
	fan_table[TWO_TERMINAL_CHOWS] = table[3];
}

// 4组顺子算番
static void calculate_4_chows(const tile_t (&mid_tiles)[4], fan_table_t &fan_table) {
	fan_t fan;
	// 存在4组顺子的番种时，不再检测其他的了
	if ((fan = get_4_chows_fan(mid_tiles[0], mid_tiles[1], mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		return;
	}

	// 3组顺子判断
	// 012构成3组顺子的番种
	if ((fan = get_3_chows_fan(mid_tiles[0], mid_tiles[1], mid_tiles[2])) != FAN_NONE) {
		fan_table[fan] = 1;
		// 计算与第4组顺子构成的番
		if ((fan = get_1_chow_extra_fan(mid_tiles[0], mid_tiles[1], mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
			fan_table[fan] = 1;
		}
		return;
	}
	// 013构成3组顺子的番种
	else if ((fan = get_3_chows_fan(mid_tiles[0], mid_tiles[1], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		// 计算与第4组顺子构成的番
		if ((fan = get_1_chow_extra_fan(mid_tiles[0], mid_tiles[1], mid_tiles[3], mid_tiles[2])) != FAN_NONE) {
			fan_table[fan] = 1;
		}
		return;
	}
	// 023构成3组顺子的番种
	else if ((fan = get_3_chows_fan(mid_tiles[0], mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		// 计算与第4组顺子构成的番
		if ((fan = get_1_chow_extra_fan(mid_tiles[0], mid_tiles[2], mid_tiles[3], mid_tiles[1])) != FAN_NONE) {
			fan_table[fan] = 1;
		}
		return;
	}
	// 123构成3组顺子的番种
	else if ((fan = get_3_chows_fan(mid_tiles[1], mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		// 计算与第4组顺子构成的番
		if ((fan = get_1_chow_extra_fan(mid_tiles[1], mid_tiles[2], mid_tiles[3], mid_tiles[0])) != FAN_NONE) {
			fan_table[fan] = 1;
		}
		return;
	}

	// 不存在3组顺子的番种时，4组顺子最多3番
	fan_t all_fans[6] = {
		get_2_chows_fan_unordered(mid_tiles[0], mid_tiles[1]),
		get_2_chows_fan_unordered(mid_tiles[0], mid_tiles[2]),
		get_2_chows_fan_unordered(mid_tiles[0], mid_tiles[3]),
		get_2_chows_fan_unordered(mid_tiles[1], mid_tiles[2]),
		get_2_chows_fan_unordered(mid_tiles[1], mid_tiles[3]),
		get_2_chows_fan_unordered(mid_tiles[2], mid_tiles[3])
	};

	int max_cnt = 3;

	// 0与其他3组顺子无任何关系
	if (all_fans[0] == FAN_NONE && all_fans[1] == FAN_NONE && all_fans[2] == FAN_NONE) {
		--max_cnt;
	}

	// 1与其他3组顺子无任何关系
	if (all_fans[0] == FAN_NONE && all_fans[3] == FAN_NONE && all_fans[4] == FAN_NONE) {
		--max_cnt;
	}

	// 2与其他3组顺子无任何关系
	if (all_fans[1] == FAN_NONE && all_fans[3] == FAN_NONE && all_fans[5] == FAN_NONE) {
		--max_cnt;
	}

	// 3与其他3组顺子无任何关系
	if (all_fans[2] == FAN_NONE && all_fans[4] == FAN_NONE && all_fans[5] == FAN_NONE) {
		--max_cnt;
	}

	if (max_cnt > 0) {
		exclusionary_rule(all_fans, 6, max_cnt, fan_table);
	}
}

// 3组顺子算番
static void calculate_3_chows(const tile_t (&mid_tiles)[3], fan_table_t &fan_table) {
	fan_t fan;

	// 存在3组顺子的番种时，不再检测其他的
	if ((fan = get_3_chows_fan(mid_tiles[0], mid_tiles[1], mid_tiles[2])) != FAN_NONE) {
		fan_table[fan] = 1;
		return;
	}

	// 不存在上述番种时，3组顺子最多2番
	fan_t all_fans[3] = {
		get_2_chows_fan_unordered(mid_tiles[0], mid_tiles[1]),
		get_2_chows_fan_unordered(mid_tiles[0], mid_tiles[2]),
		get_2_chows_fan_unordered(mid_tiles[1], mid_tiles[2])
	};
	exclusionary_rule(all_fans, 3, 2, fan_table);
}

// 2组顺子算番
static void calculate_2_chows_unordered(const tile_t (&mid_tiles)[2], fan_table_t &fan_table) {
	fan_t fan;
	if ((fan = get_2_chows_fan_unordered(mid_tiles[0], mid_tiles[1])) != FAN_NONE) {
		++fan_table[fan];
	}
}

// 刻子（杠）算番
static void calculate_kongs(const pack_t *pung_packs, intptr_t pung_cnt, fan_table_t &fan_table) {
	// 统计明杠 暗杠 明刻 暗刻
	int melded_kong_cnt = 0;
	int concealed_kong_cnt = 0;
	int concealed_pung_cnt = 0;
	for (intptr_t i = 0; i < pung_cnt; ++i) {
		if (is_pack_melded(pung_packs[i])) {
			if (pack_get_type(pung_packs[i]) == PACK_TYPE_KONG) {
				++melded_kong_cnt;
			}
		}
		else {
			if (pack_get_type(pung_packs[i]) == PACK_TYPE_KONG) {
				++concealed_kong_cnt;
			}
			else {
				++concealed_pung_cnt;
			}
		}
	}

	// 规则
	// 三杠
	// 明杠 明杠 暗杠 暗刻 -> 三杠+双暗刻+碰碰和
	// 明杠 暗杠 暗杠 明刻 -> 三杠+双暗刻+碰碰和
	// 明杠 暗杠 暗杠 暗刻 -> 三杠+三暗刻+碰碰和
	// 暗杠 暗杠 暗杠 明刻 -> 三杠+三暗刻+碰碰和
	// 暗杠 暗杠 暗杠 暗刻 -> 三杠+四暗刻
	//
	// 四杠
	// 暗杠 明杠 明杠 明杠 -> 四杠
	// 暗杠 暗杠 明杠 明杠 -> 四杠+双暗刻
	// 暗杠 暗杠 暗杠 明杠 -> 四杠+三暗刻
	// 暗杠 暗杠 暗杠 暗杠 -> 四杠+四暗刻
	//

	int kong_cnt = melded_kong_cnt + concealed_kong_cnt;
	switch (kong_cnt) {
	case 0:  // 0个杠
		switch (concealed_pung_cnt) {  // 暗刻的个数
		case 2: fan_table[TWO_CONCEALED_PUNGS] = 1; break;
		case 3: fan_table[THREE_CONCEALED_PUNGS] = 1; break;
		case 4: fan_table[FOUR_CONCEALED_PUNGS] = 1; break;
		default: break;
		}
		break;
	case 1:  // 1个杠
		if (melded_kong_cnt == 1) {  // 明杠
			fan_table[MELDED_KONG] = 1;
			switch (concealed_pung_cnt) {  // 暗刻的个数
			case 2: fan_table[TWO_CONCEALED_PUNGS] = 1; break;
			case 3: fan_table[THREE_CONCEALED_PUNGS] = 1; break;
			default: break;
			}
		}
		else {  // 暗杠
			fan_table[CONCEALED_KONG] = 1;
			switch (concealed_pung_cnt) {  // 暗刻的个数
			case 1: fan_table[TWO_CONCEALED_PUNGS] = 1; break;
			case 2: fan_table[THREE_CONCEALED_PUNGS] = 1; break;
			case 3: fan_table[FOUR_CONCEALED_PUNGS] = 1; break;
			default: break;
			}
		}
		break;
	case 2:  // 2个杠
		switch (concealed_kong_cnt) {
		case 0:  // 双明杠
			fan_table[TWO_MELDED_KONGS] = 1;
			switch (concealed_pung_cnt) {  // 暗刻的个数
			case 2: fan_table[TWO_CONCEALED_PUNGS] = 1; break;
			default: break;
			}
			break;
		case 1:  // 明暗杠
#if SUPPORT_CONCEALED_KONG_AND_MELDED_KONG
			fan_table[CONCEALED_KONG_AND_MELDED_KONG] = 1;
#else
			fan_table[MELDED_KONG] = 1;
			fan_table[CONCEALED_KONG] = 1;
#endif
			switch (concealed_pung_cnt) {  // 暗刻的个数
			case 1: fan_table[TWO_CONCEALED_PUNGS] = 1; break;
			case 2: fan_table[THREE_CONCEALED_PUNGS] = 1; break;
			default: break;
			}
			break;
		case 2:  // 双暗杠
			fan_table[TWO_CONCEALED_KONGS] = 1;
			switch (concealed_pung_cnt) {  // 暗刻的个数
			case 1: fan_table[THREE_CONCEALED_PUNGS] = 1; break;
			case 2: fan_table[FOUR_CONCEALED_PUNGS] = 1; break;
			default: break;
			}
			break;
		default:
			break;
		}
		break;
	case 3:  // 3个杠
		fan_table[THREE_KONGS] = 1;
		switch (concealed_kong_cnt) {  // 暗刻的个数
		case 0:  // 3明杠
			break;
		case 1:  // 1暗杠2明杠
			if (concealed_pung_cnt > 0) {
				fan_table[TWO_CONCEALED_PUNGS] = 1;
			}
			break;
		case 2:  // 2暗杠1明杠
			if (concealed_pung_cnt == 0) {
				fan_table[TWO_CONCEALED_PUNGS] = 1;
			}
			else {
				fan_table[THREE_CONCEALED_PUNGS] = 1;
			}
			break;
		case 3:  // 3暗杠
			if (concealed_pung_cnt == 0) {
				fan_table[THREE_CONCEALED_PUNGS] = 1;
			}
			else {
				fan_table[FOUR_CONCEALED_PUNGS] = 1;
			}
			break;
		default:
			break;
		}
		break;
	case 4:  // 4个杠
		fan_table[FOUR_KONGS] = 1;
		switch (concealed_kong_cnt) {
		case 2: fan_table[TWO_CONCEALED_PUNGS] = 1; break;
		case 3: fan_table[THREE_CONCEALED_PUNGS] = 1; break;
		case 4: fan_table[FOUR_CONCEALED_PUNGS] = 1; break;
		default: break;
		}
		break;
	default:
		UNREACHABLE();
		break;
	}

	// 四杠和四暗刻不计碰碰和，其他先加上碰碰和的番
	if (pung_cnt == 4) {
		if (fan_table[FOUR_KONGS] == 0 && fan_table[FOUR_CONCEALED_PUNGS] == 0) {
			fan_table[ALL_PUNGS] = 1;
		}
	}

	// 逐组刻子的番（箭刻、幺九刻）
	for (intptr_t i = 0; i < pung_cnt; ++i) {
		fan_t fan = get_1_pung_fan(pack_get_tile(pung_packs[i]));
		if (fan != FAN_NONE) {
			++fan_table[fan];
		}
	}
}

// 4组刻子算番
static void calculate_4_pungs(const tile_t (&mid_tiles)[4], fan_table_t &fan_table) {
	fan_t fan;
	// 存在4组刻子的番种时，不再检测其他的了
	if ((fan = get_4_pungs_fan(mid_tiles[0], mid_tiles[1], mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		return;
	}

	// 3组刻子判断
	bool _3_pungs_has_fan = false;
	int free_pack_idx = -1;  // 未使用的1组刻子
	// 012构成3组刻子的番种
	if ((fan = get_3_pungs_fan(mid_tiles[0], mid_tiles[1], mid_tiles[2])) != FAN_NONE) {
		fan_table[fan] = 1;
		free_pack_idx = 3;
		_3_pungs_has_fan = true;
	}
	// 013构成3组刻子的番种
	else if ((fan = get_3_pungs_fan(mid_tiles[0], mid_tiles[1], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		free_pack_idx = 2;
		_3_pungs_has_fan = true;
	}
	// 023构成3组刻子的番种
	else if ((fan = get_3_pungs_fan(mid_tiles[0], mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		free_pack_idx = 1;
		_3_pungs_has_fan = true;
	}
	// 123构成3组刻子的番种
	else if ((fan = get_3_pungs_fan(mid_tiles[1], mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
		fan_table[fan] = 1;
		free_pack_idx = 0;
		_3_pungs_has_fan = true;
	}

	// 存在3组刻子的番种时，余下的第4组刻子只能组合一次
	if (_3_pungs_has_fan) {
		for (int i = 0; i < 4; ++i) {
			if (i == free_pack_idx) {
				continue;
			}
			// 依次与未使用的这组刻子测试番种
			if ((fan = get_2_pungs_fan_unordered(mid_tiles[i], mid_tiles[free_pack_idx])) != FAN_NONE) {
				++fan_table[fan];
				break;
			}
		}
		return;
	}

	// 不存在3组刻子的番种时，两两计算番种
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[0], mid_tiles[1])) != FAN_NONE) {
		++fan_table[fan];
	}
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[0], mid_tiles[2])) != FAN_NONE) {
		++fan_table[fan];
	}
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[0], mid_tiles[3])) != FAN_NONE) {
		++fan_table[fan];
	}
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[1], mid_tiles[2])) != FAN_NONE) {
		++fan_table[fan];
	}
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[1], mid_tiles[3])) != FAN_NONE) {
		++fan_table[fan];
	}
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[2], mid_tiles[3])) != FAN_NONE) {
		++fan_table[fan];
	}
}

// 3组刻子算番
static void calculate_3_pungs(const tile_t (&mid_tiles)[3], fan_table_t &fan_table) {
	fan_t fan;

	// 存在3组刻子的番种（三节高 三同刻 三风刻 大三元）时，不再检测其他的
	if ((fan = get_3_pungs_fan(mid_tiles[0], mid_tiles[1], mid_tiles[2])) != FAN_NONE) {
		fan_table[fan] = 1;
		return;
	}

	// 不存在3组刻子的番种时，两两计算番种
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[0], mid_tiles[1])) != FAN_NONE) {
		++fan_table[fan];
	}
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[0], mid_tiles[2])) != FAN_NONE) {
		++fan_table[fan];
	}
	if ((fan = get_2_pungs_fan_unordered(mid_tiles[1], mid_tiles[2])) != FAN_NONE) {
		++fan_table[fan];
	}
}

// 2组刻子算番
static void calculate_2_pungs_unordered(const tile_t (&mid_tiles)[2], fan_table_t &fan_table) {
	fan_t fan = get_2_pungs_fan_unordered(mid_tiles[0], mid_tiles[1]);
	if (fan != FAN_NONE) {
		++fan_table[fan];
	}
}

// 九莲宝灯
static bool is_nine_gates(const tile_t *tiles) {
	// 对立牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(tiles, 13, &cnt_table);

	// 1、9各三枚，2~8各一枚
	return (cnt_table[0x11] == 3 && cnt_table[0x19] == 3 && std::all_of(cnt_table + 0x12, cnt_table + 0x19, [](int n) { return n == 1; }))
		|| (cnt_table[0x21] == 3 && cnt_table[0x29] == 3 && std::all_of(cnt_table + 0x22, cnt_table + 0x29, [](int n) { return n == 1; }))
		|| (cnt_table[0x31] == 3 && cnt_table[0x39] == 3 && std::all_of(cnt_table + 0x32, cnt_table + 0x39, [](int n) { return n == 1; }));
}

// 一色双龙会
static bool is_pure_terminal_chows(const pack_t (&chow_packs)[4], pack_t pair_pack) {
	if (tile_get_rank(pack_get_tile(pair_pack)) != 5) {  // 5作雀头
		return false;
	}

	int _123_cnt = 0, _789_cnt = 0;
	suit_t pair_suit = tile_get_suit(pack_get_tile(pair_pack));
	for (int i = 0; i < 4; ++i) {
		suit_t suit = tile_get_suit(pack_get_tile(chow_packs[i]));
		if (suit != pair_suit) {  // 花色与雀头相同
			return false;
		}
		rank_t rank = tile_get_rank(pack_get_tile(chow_packs[i]));
		switch (rank) {
		case 2: ++_123_cnt; break;
		case 8: ++_789_cnt; break;
		default: return false;
		}
	}
	return (_123_cnt == 2 && _789_cnt == 2);  // 123和789各2组
}

// 三色双龙会
static bool is_three_suited_terminal_chows(const pack_t (&chow_packs)[4], pack_t pair_pack) {
	if (tile_get_rank(pack_get_tile(pair_pack)) != 5) {  // 5作雀头
		return false;
	}

	int _123_suit_table[4] = { 0 };
	int _789_suit_table[4] = { 0 };
	suit_t pair_suit = tile_get_suit(pack_get_tile(pair_pack));
	for (int i = 0; i < 4; ++i) {
		suit_t suit = tile_get_suit(pack_get_tile(chow_packs[i]));
		if (suit == pair_suit) {  // 花色与雀头不相同
			return false;
		}
		rank_t rank = tile_get_rank(pack_get_tile(chow_packs[i]));
		switch (rank) {
		case 2: ++_123_suit_table[suit]; break;
		case 8: ++_789_suit_table[suit]; break;
		default: return false;
		}
	}

	// 与雀头花色不同的两色123和789各一组
	switch (pair_suit) {
	case 1: return (_123_suit_table[2] && _123_suit_table[3] && _789_suit_table[2] && _789_suit_table[3]);
	case 2: return (_123_suit_table[1] && _123_suit_table[3] && _789_suit_table[1] && _789_suit_table[3]);
	case 3: return (_123_suit_table[1] && _123_suit_table[2] && _789_suit_table[1] && _789_suit_table[2]);
	default: return false;
	}
}

// 根据和牌方式调整——涉及番种：不求人、全求人
static void adjust_by_self_drawn(const pack_t (&packs)[5], intptr_t fixed_cnt, bool self_drawn, fan_table_t &fan_table) {
	ptrdiff_t melded_cnt = std::count_if(&packs[0], &packs[fixed_cnt], &is_pack_melded);  // 明副露的组数

	switch (melded_cnt) {
	case 0:  // 0组明的，自摸为不求人，点和为门前清
		fan_table[self_drawn ? FULLY_CONCEALED_HAND : CONCEALED_HAND] = 1;
		break;
	case 4:
		// 4组明的，自摸还是自摸，点和为全求人
		fan_table[self_drawn ? SELF_DRAWN : MELDED_HAND] = 1;
		break;
	default:
		if (self_drawn) {
			fan_table[SELF_DRAWN] = 1;
		}
		break;
	}
}

// 根据雀头调整——涉及番种：平和、小三元、小四喜
static void adjust_by_pair_tile(tile_t pair_tile, intptr_t chow_cnt, fan_table_t &fan_table) {
	if (chow_cnt == 4) {  // 4组都是顺子
		if (is_numbered_suit_quick(pair_tile)) {  // 数牌雀头
			fan_table[ALL_CHOWS] = 1;  // 平和
		}
		return;
	}

	// 在双箭刻的基础上，如果雀头是箭牌，则修正为小三元
	if (fan_table[TWO_DRAGONS_PUNGS]) {
		if (is_dragons(pair_tile)) {
			fan_table[LITTLE_THREE_DRAGONS] = 1;
			fan_table[TWO_DRAGONS_PUNGS] = 0;
		}
		return;
	}
	// 在三风刻的基础上，如果雀头是风牌，则修正为小四喜
	if (fan_table[BIG_THREE_WINDS]) {
		if (is_winds(pair_tile)) {
			fan_table[LITTLE_FOUR_WINDS] = 1;
			fan_table[BIG_THREE_WINDS] = 0;
		}
		return;
	}
}

// 根据花色调整——涉及番种：无字、缺一门、混一色、清一色、五门齐
static void adjust_by_suits(const tile_t *tiles, intptr_t tile_cnt, fan_table_t &fan_table) {
	// 打表标记有哪些花色，用bit操作
	uint8_t suit_flag = 0;
	for (intptr_t i = 0; i < tile_cnt; ++i) {
		suit_flag |= (1 << tile_get_suit(tiles[i]));
	}

	// 1111 0001
	if (!(suit_flag & 0xF1U)) {
		fan_table[NO_HONORS] = 1;  // 无字
	}

	// 1110 0011
	if (!(suit_flag & 0xE3U)) {
		++fan_table[ONE_VOIDED_SUIT];  // 缺一门（万）
	}
	// 1110 0101
	if (!(suit_flag & 0xE5U)) {
		++fan_table[ONE_VOIDED_SUIT];  // 缺一门（条）
	}
	// 1110 1001
	if (!(suit_flag & 0xE9U)) {
		++fan_table[ONE_VOIDED_SUIT];  // 缺一门（饼）
	}

	// 当缺2门时，根据有字和无字，修正为混一色和清一色
	if (fan_table[ONE_VOIDED_SUIT] == 2) {
		fan_table[ONE_VOIDED_SUIT] = 0;
		fan_table[suit_flag & 0xF1U ? HALF_FLUSH : FULL_FLUSH] = 1;
	}

	// 0001 1110
	if (suit_flag == 0x1EU) {  // 三门数牌和字牌都有
		if (std::any_of(tiles, tiles + tile_cnt, &is_winds)
			&& std::any_of(tiles, tiles + tile_cnt, &is_dragons)) {
			fan_table[ALL_TYPES] = 1;  // 五门齐
		}
	}
}

// 根据数牌的范围调整——涉及番种：大于五、小于五、全大、全中、全小
static void adjust_by_rank_range(const tile_t *tiles, intptr_t tile_cnt, fan_table_t &fan_table) {
#ifdef STRICT_98_RULE
	if (fan_table[SEVEN_PAIRS]) {
		return;  // 严格98规则的七对不支持叠加这些
	}
#endif

	// 打表标记有哪些数
	uint16_t rank_flag = 0;
	for (intptr_t i = 0; i < tile_cnt; ++i) {
		if (!is_numbered_suit_quick(tiles[i])) {
			return;
		}
		rank_flag |= (1 << tile_get_rank(tiles[i]));
	}

	// 1111 1111 1110 0001
	// 检测是否只包含1234
	if (!(rank_flag & 0xFFE1)) {
		// 包含4为小于五，否则为全小
		fan_table[rank_flag & 0x0010 ? LOWER_FOUR : LOWER_TILES] = 1;
		return;
	}
	// 1111 1100 0011 1111
	// 检测是否只包含6789
	if (!(rank_flag & 0xFC3F)) {
		// 包含6为大于五，否则为全大
		fan_table[rank_flag & 0x0040 ? UPPER_FOUR : UPPER_TILES] = 1;
		return;
	}
	// 1111 1111 1000 1111
	// 检测是否只包含456
	if (!(rank_flag & 0xFF8F)) {
		// 全中
		fan_table[MIDDLE_TILES] = 1;
	}
}

// 根据牌组特征调整——涉及番种：全带幺、全带五、全双刻
static void adjust_by_packs_traits(const pack_t (&packs)[5], fan_table_t &fan_table) {
	// 统计包含数牌19、字牌、5、双数牌的组数
	int terminal_pack = 0;
	int honor_pack = 0;
	int _5_pack = 0;
	int even_pack = 0;
	for (int i = 0; i < 5; ++i) {
		tile_t tile = pack_get_tile(packs[i]);
		if (is_numbered_suit_quick(tile)) {  // 数牌
			rank_t rank = tile_get_rank(tile);
			if (pack_get_type(packs[i]) == PACK_TYPE_CHOW) {  // 顺子
				switch (rank) {
				case 2: case 8: ++terminal_pack; break;  // 数牌19
				case 4: case 5: case 6: ++_5_pack; break;  // 带五
				default: break;
				}
			}
			else {  // 刻子或雀头
				switch (rank) {
				case 1: case 9: ++terminal_pack; break;  // 数牌19
				case 5: ++_5_pack; break;  // 带五
				case 2: case 4: case 6: case 8: ++even_pack; break;  // 双刻
				default: break;
				}
			}
		}
		else {
			++honor_pack;  // 字牌
		}
	}

	// 5组牌都包含数牌19和字牌，先暂时计为全带幺
	if (terminal_pack + honor_pack == 5) {
		fan_table[OUTSIDE_HAND] = 1;
		return;
	}
	// 全带五
	if (_5_pack == 5) {
		fan_table[ALL_FIVE] = 1;
		return;
	}
	// 全双刻
	if (even_pack == 5) {
		fan_table[ALL_EVEN_PUNGS] = 1;
	}
}

// 根据牌特性调整——涉及番种：断幺、推不倒、绿一色、字一色、清幺九、混幺九
static void adjust_by_tiles_traits(const tile_t *tiles, intptr_t tile_cnt, fan_table_t &fan_table) {
	// 断幺
	if (std::none_of(tiles, tiles + tile_cnt, &is_terminal_or_honor)) {
		fan_table[ALL_SIMPLES] = 1;
	}

	// 推不倒
	if (std::all_of(tiles, tiles + tile_cnt, &is_reversible)) {
		fan_table[REVERSIBLE_TILES] = 1;
	}

#ifdef STRICT_98_RULE
	if (fan_table[SEVEN_PAIRS]) {
		return;  // 严格98规则的七对不支持绿一色、字一色、清幺九、混幺九
	}
#endif

	// 绿一色
	if (std::all_of(tiles, tiles + tile_cnt, &is_green)) {
		fan_table[ALL_GREEN] = 1;
	}

	// 如果断幺了就没必要检测字一色、清幺九、混幺九了
	if (fan_table[ALL_SIMPLES] != 0) {
		return;
	}

	// 字一色
	if (std::all_of(tiles, tiles + tile_cnt, &is_honor)) {
		fan_table[ALL_HONORS] = 1;
		return;
	}
	// 清幺九
	if (std::all_of(tiles, tiles + tile_cnt, &is_terminal)) {
		fan_table[ALL_TERMINALS] = 1;
		return;
	}
	// 混幺九
	if (std::all_of(tiles, tiles + tile_cnt, &is_terminal_or_honor)) {
		fan_table[ALL_TERMINALS_AND_HONORS] = 1;
	}
}

// 四归一调整
static void adjust_by_tiles_hog(const tile_t *tiles, intptr_t tile_cnt, fan_table_t &fan_table) {
	intptr_t kong_cnt = tile_cnt - 14;  // 标准和牌14张，多出几张就说明有几个杠
	tile_table_t cnt_table;
	map_tiles(tiles, tile_cnt, &cnt_table);
	// 有多少种已经用去4张的牌减去杠的数量，即为四归一的数量
	intptr_t _4_cnt = std::count(std::begin(cnt_table), std::end(cnt_table), 4);
	fan_table[TILE_HOG] = static_cast<uint8_t>(_4_cnt - kong_cnt);
}

// 根据听牌方式调整——涉及番种：边张、嵌张、单钓将
static void adjust_by_waiting_form(const pack_t *concealed_packs, intptr_t pack_cnt, const tile_t *standing_tiles, intptr_t standing_cnt,
	tile_t win_tile, fan_table_t &fan_table) {
	// 全求人和四杠不计单钓将，也不可能有边张、嵌张
	if (fan_table[MELDED_HAND] || fan_table[FOUR_KONGS]) {
		return;
	}

	useful_table_t waiting_table;  // 听牌标记表
	if (!is_basic_form_wait(standing_tiles, standing_cnt, &waiting_table)) {
		return;
	}

	if (pack_cnt == 5) {  // 门清状态
		// 判断是否为七对听牌
		useful_table_t temp_table;
		if (is_seven_pairs_wait(standing_tiles, standing_cnt, &temp_table)) {
			// 合并听牌标记表
			std::transform(std::begin(temp_table), std::end(temp_table), std::begin(waiting_table),
				std::begin(waiting_table), [](bool w, bool t) { return w || t; });
		}
	}

	// 统计听牌张数，听牌数大于1张，不计边张、嵌张、单钓将
	if (1 != std::count(std::begin(waiting_table), std::end(waiting_table), true)) {
		return;
	}

	// 听1张的情况，看和牌张处于什么位置
	// 边张0x01 嵌张0x02 单钓将0x04
	uint8_t pos_flag = 0;

	for (intptr_t i = 0; i < pack_cnt; ++i) {
		switch (pack_get_type(concealed_packs[i])) {
		case PACK_TYPE_CHOW: {
			tile_t mid_tile = pack_get_tile(concealed_packs[i]);
			if (mid_tile == win_tile) {
				pos_flag |= 0x02U;  // 嵌张
			}
			else if (mid_tile + 1 == win_tile || mid_tile - 1 == win_tile) {
				pos_flag |= 0x01U;  // 边张
			}
			break;
		}
		case PACK_TYPE_PAIR: {
			tile_t mid_tile = pack_get_tile(concealed_packs[i]);
			if (mid_tile == win_tile) {
				pos_flag |= 0x04U;  // 单钓将
			}
			break;
		}
		default:
			break;
		}
	}

	// 当多种可能存在时，只能计其中一种
	if (pos_flag & 0x01U) {
		fan_table[EDGE_WAIT] = 1;
	}
	else if (pos_flag & 0x02U) {
		fan_table[CLOSED_WAIT] = 1;
	}
	else if (pos_flag & 0x04U) {
		fan_table[SINGLE_WAIT] = 1;
	}
}

// 统一调整一些不计的
static void adjust_fan_table(fan_table_t &fan_table) {
	// 大四喜不计三风刻、碰碰和、圈风刻、门风刻、幺九刻
	if (fan_table[BIG_FOUR_WINDS]) {
		fan_table[BIG_THREE_WINDS] = 0;
		fan_table[ALL_PUNGS] = 0;
		fan_table[PUNG_OF_TERMINALS_OR_HONORS] = 0;
	}
	// 大三元不计双箭刻、箭刻（严格98规则不计缺一门）
	if (fan_table[BIG_THREE_DRAGONS]) {
		fan_table[TWO_DRAGONS_PUNGS] = 0;
		fan_table[DRAGON_PUNG] = 0;
#ifdef STRICT_98_RULE
		fan_table[ONE_VOIDED_SUIT] = 0;
#endif
	}
	// 绿一色不计混一色、缺一门
	if (fan_table[ALL_GREEN]) {
		fan_table[HALF_FLUSH] = 0;
		fan_table[ONE_VOIDED_SUIT] = 0;
	}
	// 九莲宝灯不计清一色、门前清、缺一门、无字，减计1个幺九刻，把不求人修正为自摸
	if (fan_table[NINE_GATES]) {
		fan_table[FULL_FLUSH] = 0;
		fan_table[CONCEALED_HAND] = 0;
		--fan_table[PUNG_OF_TERMINALS_OR_HONORS];
		fan_table[ONE_VOIDED_SUIT] = 0;
		fan_table[NO_HONORS] = 0;
		if (fan_table[FULLY_CONCEALED_HAND]) {
			fan_table[FULLY_CONCEALED_HAND] = 0;
			fan_table[SELF_DRAWN] = 1;
		}
	}
	// 四杠不计单钓将
	if (fan_table[FOUR_KONGS]) {
		fan_table[SINGLE_WAIT] = 0;
	}
	// 连七对不计七对、清一色、门前清、缺一门、无字
	if (fan_table[SEVEN_SHIFTED_PAIRS]) {
		fan_table[SEVEN_PAIRS] = 0;
		fan_table[FULL_FLUSH] = 0;
		fan_table[CONCEALED_HAND] = 0;
		fan_table[ONE_VOIDED_SUIT] = 0;
		fan_table[NO_HONORS] = 0;
	}
	// 十三幺不计五门齐、门前清、单钓将
	if (fan_table[THIRTEEN_ORPHANS]) {
		fan_table[ALL_TYPES] = 0;
		fan_table[CONCEALED_HAND] = 0;
		fan_table[SINGLE_WAIT] = 0;
	}

	// 清幺九不计混幺九、碰碰胡、全带幺、幺九刻、无字（严格98规则不计双同刻、不计三同刻）
	if (fan_table[ALL_TERMINALS]) {
		fan_table[ALL_TERMINALS_AND_HONORS] = 0;
		fan_table[ALL_PUNGS] = 0;
		fan_table[OUTSIDE_HAND] = 0;
		fan_table[PUNG_OF_TERMINALS_OR_HONORS] = 0;
		fan_table[NO_HONORS] = 0;
		fan_table[DOUBLE_PUNG] = 0;  // 通行计法不计双同刻
#ifdef STRICT_98_RULE
		fan_table[TRIPLE_PUNG] = 0;
		fan_table[DOUBLE_PUNG] = 0;
#endif
	}

	// 小四喜不计三风刻
	if (fan_table[LITTLE_FOUR_WINDS]) {
		fan_table[BIG_THREE_WINDS] = 0;
		// 小四喜的第四组牌如果是19的刻子，则是混幺九；如果是箭刻则是字一色；这两种都是不计幺九刻的
		// 如果是顺子或者2-8的刻子，则不存在多余的幺九刻
		// 所以这里将幺九刻置为0
		fan_table[PUNG_OF_TERMINALS_OR_HONORS] = 0;
	}

	// 小三元不计双箭刻、箭刻（严格98规则不计缺一门）
	if (fan_table[LITTLE_THREE_DRAGONS]) {
		fan_table[TWO_DRAGONS_PUNGS] = 0;
		fan_table[DRAGON_PUNG] = 0;
#ifdef STRICT_98_RULE
		fan_table[ONE_VOIDED_SUIT] = 0;
#endif
	}

	// 字一色不计混幺九、碰碰胡、全带幺、幺九刻、缺一门
	if (fan_table[ALL_HONORS]) {
		fan_table[ALL_TERMINALS_AND_HONORS] = 0;
		fan_table[ALL_PUNGS] = 0;
		fan_table[OUTSIDE_HAND] = 0;
		fan_table[PUNG_OF_TERMINALS_OR_HONORS] = 0;
		fan_table[ONE_VOIDED_SUIT] = 0;
	}
	// 四暗刻不计碰碰和、门前清，把不求人修正为自摸
	if (fan_table[FOUR_CONCEALED_PUNGS]) {
		fan_table[ALL_PUNGS] = 0;
		fan_table[CONCEALED_HAND] = 0;
		if (fan_table[FULLY_CONCEALED_HAND]) {
			fan_table[FULLY_CONCEALED_HAND] = 0;
			fan_table[SELF_DRAWN] = 1;
		}
	}
	// 一色双龙会不计七对、清一色、平和、一般高、老少副、缺一门、无字
	if (fan_table[PURE_TERMINAL_CHOWS]) {
		fan_table[SEVEN_PAIRS] = 0;
		fan_table[FULL_FLUSH] = 0;
		fan_table[ALL_CHOWS] = 0;
		fan_table[PURE_DOUBLE_CHOW] = 0;
		fan_table[TWO_TERMINAL_CHOWS] = 0;
		fan_table[ONE_VOIDED_SUIT] = 0;
		fan_table[NO_HONORS] = 0;
	}

	// 一色四同顺不计一色三同顺、一般高、四归一（严格98规则不计缺一门）
	if (fan_table[QUADRUPLE_CHOW]) {
		fan_table[PURE_SHIFTED_PUNGS] = 0;
		fan_table[TILE_HOG] = 0;
		fan_table[PURE_DOUBLE_CHOW] = 0;
#ifdef STRICT_98_RULE
		fan_table[ONE_VOIDED_SUIT] = 0;
#endif
	}
	// 一色四节高不计一色三节高、碰碰和（严格98规则不计缺一门）
	if (fan_table[FOUR_PURE_SHIFTED_PUNGS]) {
		fan_table[PURE_TRIPLE_CHOW] = 0;
		fan_table[ALL_PUNGS] = 0;
#ifdef STRICT_98_RULE
		fan_table[ONE_VOIDED_SUIT] = 0;
#endif
	}

	// 一色四步高不计一色三步高、老少副、连六（严格98规则不计缺一门）
	if (fan_table[FOUR_PURE_SHIFTED_CHOWS]) {
		fan_table[PURE_SHIFTED_CHOWS] = 0;
		fan_table[TWO_TERMINAL_CHOWS] = 0;
		fan_table[SHORT_STRAIGHT] = 0;
#ifdef STRICT_98_RULE
		fan_table[ONE_VOIDED_SUIT] = 0;
#endif
	}

	// 混幺九不计碰碰和、全带幺、幺九刻
	if (fan_table[ALL_TERMINALS_AND_HONORS]) {
		fan_table[ALL_PUNGS] = 0;
		fan_table[OUTSIDE_HAND] = 0;
		fan_table[PUNG_OF_TERMINALS_OR_HONORS] = 0;
	}

	// 七对不计门前清、单钓将
	if (fan_table[SEVEN_PAIRS]) {
		fan_table[CONCEALED_HAND] = 0;
		fan_table[SINGLE_WAIT] = 0;
	}
	// 七星不靠不计五门齐、门前清
	if (fan_table[GREATER_HONORS_AND_KNITTED_TILES]) {
		fan_table[ALL_TYPES] = 0;
		fan_table[CONCEALED_HAND] = 0;
	}
	// 全双刻不计碰碰胡、断幺、无字
	if (fan_table[ALL_EVEN_PUNGS]) {
		fan_table[ALL_PUNGS] = 0;
		fan_table[ALL_SIMPLES] = 0;
		fan_table[NO_HONORS] = 0;
	}
	// 清一色不计缺一门、无字
	if (fan_table[FULL_FLUSH]) {
		fan_table[ONE_VOIDED_SUIT] = 0;
		fan_table[NO_HONORS] = 0;
	}
	// 一色三同顺不计一色三节高、一般高
	if (fan_table[PURE_TRIPLE_CHOW]) {
		fan_table[PURE_SHIFTED_PUNGS] = 0;
		fan_table[PURE_DOUBLE_CHOW] = 0;
	}
	// 一色三节高不计一色三同顺
	if (fan_table[PURE_SHIFTED_PUNGS]) {
		fan_table[PURE_TRIPLE_CHOW] = 0;
	}
	// 全大不计大于五、无字
	if (fan_table[UPPER_TILES]) {
		fan_table[UPPER_FOUR] = 0;
		fan_table[NO_HONORS] = 0;
	}
	// 全中不计断幺
	if (fan_table[MIDDLE_TILES]) {
		fan_table[ALL_SIMPLES] = 0;
		fan_table[NO_HONORS] = 0;
	}
	// 全小不计小于五、无字
	if (fan_table[LOWER_TILES]) {
		fan_table[LOWER_FOUR] = 0;
		fan_table[NO_HONORS] = 0;
	}

	// 三色双龙会不计平和、无字、喜相逢、老少副
	if (fan_table[THREE_SUITED_TERMINAL_CHOWS]) {
		fan_table[ALL_CHOWS] = 0;
		fan_table[NO_HONORS] = 0;
		fan_table[MIXED_DOUBLE_CHOW] = 0;
		fan_table[TWO_TERMINAL_CHOWS] = 0;
	}
	// 全带五不计断幺、无字
	if (fan_table[ALL_FIVE]) {
		fan_table[ALL_SIMPLES] = 0;
		fan_table[NO_HONORS] = 0;
	}

	// 七星不靠不计五门齐、门前清
	if (fan_table[LESSER_HONORS_AND_KNITTED_TILES]) {
		fan_table[ALL_TYPES] = 0;
		fan_table[CONCEALED_HAND] = 0;
	}
	// 大于五不计无字
	if (fan_table[UPPER_FOUR]) {
		fan_table[NO_HONORS] = 0;
	}
	// 小于五不计无字
	if (fan_table[LOWER_FOUR]) {
		fan_table[NO_HONORS] = 0;
	}
	// 三风刻内部不再计幺九刻（严格98规则不计缺一门）
	if (fan_table[BIG_THREE_WINDS]) {
		// 如果不是字一色或混幺九，则要减去3个幺九刻
		if (!fan_table[ALL_HONORS] && !fan_table[ALL_TERMINALS_AND_HONORS]) {
			assert(fan_table[PUNG_OF_TERMINALS_OR_HONORS] >= 3);
			fan_table[PUNG_OF_TERMINALS_OR_HONORS] -= 3;
		}
#ifdef STRICT_98_RULE
		fan_table[ONE_VOIDED_SUIT] = 0;
#endif
	}

	// 推不倒不计缺一门
	if (fan_table[REVERSIBLE_TILES]) {
		fan_table[ONE_VOIDED_SUIT] = 0;
	}
	// 妙手回春不计自摸
	if (fan_table[LAST_TILE_DRAW]) {
		fan_table[SELF_DRAWN] = 0;
	}
	// 杠上开花不计自摸
	if (fan_table[OUT_WITH_REPLACEMENT_TILE]) {
		fan_table[SELF_DRAWN] = 0;
	}
	// 抢杠和不计和绝张
	if (fan_table[ROBBING_THE_KONG]) {
		fan_table[LAST_TILE] = 0;
	}
	// 双暗杠不计暗杠
	if (fan_table[TWO_CONCEALED_KONGS]) {
		fan_table[CONCEALED_KONG] = 0;
	}

	// 混一色不计缺一门
	if (fan_table[HALF_FLUSH]) {
		fan_table[ONE_VOIDED_SUIT] = 0;
	}
	// 全求人不计单钓将
	if (fan_table[MELDED_HAND]) {
		fan_table[SINGLE_WAIT] = 0;
	}
	// 双箭刻不计箭刻
	if (fan_table[TWO_DRAGONS_PUNGS]) {
		fan_table[DRAGON_PUNG] = 0;
	}

	// 不求人不计自摸
	if (fan_table[FULLY_CONCEALED_HAND]) {
		fan_table[SELF_DRAWN] = 0;
	}
	// 双明杠不计明杠
	if (fan_table[TWO_MELDED_KONGS]) {
		fan_table[MELDED_KONG] = 0;
	}

	// 平和不计无字
	if (fan_table[ALL_CHOWS]) {
		fan_table[NO_HONORS] = 0;
	}
	// 断幺不计无字
	if (fan_table[ALL_SIMPLES]) {
		fan_table[NO_HONORS] = 0;
	}
}

// 调整圈风刻、门风刻
static void adjust_by_winds(tile_t tile, wind_t prevalent_wind, wind_t seat_wind, fan_table_t &fan_table) {
	// 三风刻、混幺九、字一色、小四喜，这些番种已经扣除过幺九刻了
	bool is_deducted = (fan_table[BIG_THREE_WINDS] || fan_table[ALL_TERMINALS_AND_HONORS] || fan_table[ALL_HONORS] || fan_table[LITTLE_FOUR_WINDS]);

	rank_t delta = tile - TILE_E;
	if (delta == static_cast<int>(prevalent_wind) - static_cast<int>(wind_t::EAST)) {
		fan_table[PREVALENT_WIND] = 1;
		if (!is_deducted) {
			--fan_table[PUNG_OF_TERMINALS_OR_HONORS];
		}
	}
	if (delta == static_cast<int>(seat_wind) - static_cast<int>(wind_t::EAST)) {
		fan_table[SEAT_WIND] = 1;
		// 当圈风门风相同时，已经扣除过幺九刻了
		if (seat_wind != prevalent_wind && !is_deducted) {
			--fan_table[PUNG_OF_TERMINALS_OR_HONORS];
		}
	}
}

// 根据和牌标记调整——涉及番种：和绝张、妙手回春、海底捞月、自摸
static void adjust_by_win_flag(win_flag_t win_flag, fan_table_t &fan_table) {
	if (win_flag & WIN_FLAG_4TH_TILE) {
		fan_table[LAST_TILE] = 1;
	}
	if (win_flag & WIN_FLAG_WALL_LAST) {
		fan_table[win_flag & WIN_FLAG_SELF_DRAWN ? LAST_TILE_DRAW : LAST_TILE_CLAIM] = 1;
	}
	if (win_flag & WIN_FLAG_ABOUT_KONG) {
		fan_table[win_flag & WIN_FLAG_SELF_DRAWN ? OUT_WITH_REPLACEMENT_TILE : ROBBING_THE_KONG] = 1;
	}
	if (win_flag & WIN_FLAG_SELF_DRAWN) {
		fan_table[SELF_DRAWN] = 1;
	}
}

// 基本和型算番
static void calculate_basic_form_fan(const pack_t (&packs)[5], const calculate_param_t *calculate_param, win_flag_t win_flag, fan_table_t &fan_table) {
	pack_t pair_pack = 0;
	pack_t chow_packs[4];
	pack_t pung_packs[4];
	intptr_t chow_cnt = 0;
	intptr_t pung_cnt = 0;
	for (int i = 0; i < 5; ++i) {
		switch (pack_get_type(packs[i])) {
		case PACK_TYPE_CHOW: chow_packs[chow_cnt++] = packs[i]; break;
		case PACK_TYPE_PUNG:
		case PACK_TYPE_KONG: pung_packs[pung_cnt++] = packs[i]; break;
		case PACK_TYPE_PAIR: pair_pack = packs[i]; break;
		default: UNREACHABLE(); return;
		}
	}

	if (pair_pack == 0 || chow_cnt + pung_cnt != 4) {
		return;
	}

	tile_t win_tile = calculate_param->win_tile;

	// 根据和牌标记调整——涉及番种：和绝张、妙手回春、海底捞月、自摸
	adjust_by_win_flag(win_flag, fan_table);

	// 点和的牌张，如果不能解释为顺子中的一张，那么将其解释为刻子，并标记这个刻子为明刻
	if ((win_flag & WIN_FLAG_SELF_DRAWN) == 0) {
		// 和牌不能解释为顺子中的一张
		if (std::none_of(chow_packs, chow_packs + chow_cnt, [win_tile](pack_t chow_pack) {
			tile_t tile = pack_get_tile(chow_pack);
			return !is_pack_melded(chow_pack)
				&& (tile - 1 == win_tile || tile == win_tile || tile + 1 == win_tile);
		})) {
			for (intptr_t i = 0; i < pung_cnt; ++i) {
				if (pack_get_tile(pung_packs[i]) == win_tile && !is_pack_melded(pung_packs[i])) {
					pung_packs[i] |= (1 << 12);  // 标记为明副露
				}
			}
		}
	}

	if (pung_cnt > 0) { // 有刻子
		calculate_kongs(pung_packs, pung_cnt, fan_table);
	}

	switch (chow_cnt) {
	case 4: {  // 4组顺子
		// 检测三色/一色双龙会
		if (is_three_suited_terminal_chows(chow_packs, pair_pack)) {
			fan_table[THREE_SUITED_TERMINAL_CHOWS] = 1;
			break;
		}
		if (is_pure_terminal_chows(chow_packs, pair_pack)) {
			fan_table[PURE_TERMINAL_CHOWS] = 1;
			break;
		}

		tile_t mid_tiles[4];
		mid_tiles[0] = pack_get_tile(chow_packs[0]);
		mid_tiles[1] = pack_get_tile(chow_packs[1]);
		mid_tiles[2] = pack_get_tile(chow_packs[2]);
		mid_tiles[3] = pack_get_tile(chow_packs[3]);
		std::sort(std::begin(mid_tiles), std::end(mid_tiles));

		calculate_4_chows(mid_tiles, fan_table);
		break;
	}
	case 3: {  // 3组顺子+1组刻子
		tile_t mid_tiles[3];
		mid_tiles[0] = pack_get_tile(chow_packs[0]);
		mid_tiles[1] = pack_get_tile(chow_packs[1]);
		mid_tiles[2] = pack_get_tile(chow_packs[2]);
		std::sort(std::begin(mid_tiles), std::end(mid_tiles));

		calculate_3_chows(mid_tiles, fan_table);
		break;
	}
	case 2: {  // 2组顺子+2组刻子
		tile_t mid_tiles_chow[2];
		mid_tiles_chow[0] = pack_get_tile(chow_packs[0]);
		mid_tiles_chow[1] = pack_get_tile(chow_packs[1]);

		tile_t mid_tiles_pung[2];
		mid_tiles_pung[0] = pack_get_tile(pung_packs[0]);
		mid_tiles_pung[1] = pack_get_tile(pung_packs[1]);

		calculate_2_chows_unordered(mid_tiles_chow, fan_table);
		calculate_2_pungs_unordered(mid_tiles_pung, fan_table);
		break;
	}
	case 1: {  // 1组顺子+3组刻子
		tile_t mid_tiles[3];
		mid_tiles[0] = pack_get_tile(pung_packs[0]);
		mid_tiles[1] = pack_get_tile(pung_packs[1]);
		mid_tiles[2] = pack_get_tile(pung_packs[2]);
		std::sort(std::begin(mid_tiles), std::end(mid_tiles));

		calculate_3_pungs(mid_tiles, fan_table);
		break;
	}
	case 0: {  // 4组刻子
		tile_t mid_tiles[4];
		mid_tiles[0] = pack_get_tile(pung_packs[0]);
		mid_tiles[1] = pack_get_tile(pung_packs[1]);
		mid_tiles[2] = pack_get_tile(pung_packs[2]);
		mid_tiles[3] = pack_get_tile(pung_packs[3]);
		std::sort(std::begin(mid_tiles), std::end(mid_tiles));

		calculate_4_pungs(mid_tiles, fan_table);
		break;
	}
	default:
		UNREACHABLE();
		break;
	}

	intptr_t fixed_cnt = calculate_param->hand_tiles.pack_count;
	const tile_t *standing_tiles = calculate_param->hand_tiles.standing_tiles;
	intptr_t standing_cnt = calculate_param->hand_tiles.tile_count;
	wind_t prevalent_wind = calculate_param->prevalent_wind;
	wind_t seat_wind = calculate_param->seat_wind;

	bool heaven_win = (win_flag & (WIN_FLAG_INIT | WIN_FLAG_SELF_DRAWN)) == (WIN_FLAG_INIT | WIN_FLAG_SELF_DRAWN);

	// 九莲宝灯
	if (!heaven_win && standing_cnt == 13) {
		if (is_nine_gates(standing_tiles)) {
			fan_table[NINE_GATES] = 1;
		}
	}

	// 根据和牌方式调整——涉及番种：不求人、全求人
	adjust_by_self_drawn(packs, fixed_cnt, (win_flag & WIN_FLAG_SELF_DRAWN) != 0, fan_table);
	// 根据雀头调整——涉及番种：平和、小三元、小四喜
	adjust_by_pair_tile(pack_get_tile(pair_pack), chow_cnt, fan_table);
	// 根据牌组特征调整——涉及番种：全带幺、全带五、全双刻
	adjust_by_packs_traits(packs, fan_table);

	tile_t tiles[18];
	memcpy(tiles, standing_tiles, standing_cnt * sizeof(tile_t));
	intptr_t tile_cnt = packs_to_tiles(packs, fixed_cnt, &tiles[standing_cnt], 18 - standing_cnt);
	tile_cnt += standing_cnt;
	tiles[tile_cnt++] = win_tile;

	// 根据花色调整——涉及番种：无字、缺一门、混一色、清一色、五门齐
	adjust_by_suits(tiles, tile_cnt, fan_table);
	// 根据牌特性调整——涉及番种：断幺、推不倒、绿一色、字一色、清幺九、混幺九
	adjust_by_tiles_traits(tiles, tile_cnt, fan_table);
	// 根据数牌的范围调整——涉及番种：大于五、小于五、全大、全中、全小
	adjust_by_rank_range(tiles, tile_cnt, fan_table);
	// 四归一调整
	adjust_by_tiles_hog(tiles, tile_cnt, fan_table);

	if (!heaven_win) {
		// 根据听牌方式调整——涉及番种：边张、嵌张、单钓将
		adjust_by_waiting_form(packs + fixed_cnt, 5 - fixed_cnt, standing_tiles, standing_cnt, win_tile, fan_table);
	}

	// 统一调整一些不计的
	adjust_fan_table(fan_table);

	// 调整圈风刻、门风刻（大四喜不计圈风刻、门风刻）
	if (fan_table[BIG_FOUR_WINDS] == 0) {
		for (intptr_t i = 0; i < pung_cnt; ++i) {
			tile_t tile = pack_get_tile(pung_packs[i]);
			if (is_winds(tile)) {
				adjust_by_winds(tile, prevalent_wind, seat_wind, fan_table);
			}
		}
	}

	// 如果什么番都没有，则计为无番和
	if (std::all_of(std::begin(fan_table), std::end(fan_table), [](uint16_t p) { return p == 0; })) {
		fan_table[CHICKEN_HAND] = 1;
	}
}

// “组合龙+面子+雀头”和型算番
static bool calculate_knitted_straight_fan(const calculate_param_t *calculate_param, win_flag_t win_flag, fan_table_t &fan_table) {
	const hand_tiles_t *hand_tiles = &calculate_param->hand_tiles;
	tile_t win_tile = calculate_param->win_tile;
	wind_t prevalent_wind = calculate_param->prevalent_wind;
	wind_t seat_wind = calculate_param->seat_wind;

	intptr_t fixed_cnt = hand_tiles->pack_count;
	if (fixed_cnt > 1) {
		return false;
	}

	const pack_t *fixed_packs = hand_tiles->fixed_packs;
	intptr_t standing_cnt = hand_tiles->tile_count;

	// 对立牌和和牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(hand_tiles->standing_tiles, standing_cnt, &cnt_table);
	++cnt_table[win_tile];

	// 匹配组合龙
	const tile_t (*matched_seq)[9] = std::find_if(&standard_knitted_straight[0], &standard_knitted_straight[6],
		[&cnt_table](const tile_t (&seq)[9]) {
		return std::all_of(std::begin(seq), std::end(seq), [&cnt_table](tile_t t) { return cnt_table[t] > 0; });
	});

	if (matched_seq == &standard_knitted_straight[6]) {
		return false;
	}

	// 剔除组合龙
	std::for_each(std::begin(*matched_seq), std::end(*matched_seq), [&cnt_table](tile_t t) { --cnt_table[t]; });

	// 按基本和型划分
	division_result_t result;
	result.count = 0;
	division_t work_division;
	memset(&work_division, 0, sizeof(work_division));

	// 此处逻辑为：将组合龙9张牌当作是已经完成的3组面子，空出0 1 2下标处的3组
	// 如果第4组是副露的，将其放在下标3处
	// 然后按基本和型从从fixed_cnt + 3开始递归
	// 划分后的结果，下标3处为第四组面子，下标4处为雀头
	if (fixed_cnt == 1) {
		work_division.packs[3] = fixed_packs[0];
	}
	divide_recursively(cnt_table, fixed_cnt + 3, 0, &work_division, &result);
	if (result.count != 1) {
		return false;
	}

	const pack_t *packs = result.divisions[0].packs;  // packs的0 1 2下标都是没用的

	// 标记番
	fan_table[KNITTED_STRAIGHT] = 1;  // 组合龙
	if (pack_get_type(packs[3]) == PACK_TYPE_CHOW) {  // 第4组是顺子
		if (is_numbered_suit_quick(pack_get_tile(packs[4]))) {
			fan_table[ALL_CHOWS] = 1;  // 雀头是数牌时，为平和
		}
	}
	else {
		calculate_kongs(&packs[3], 1, fan_table);
	}

	adjust_by_win_flag(win_flag, fan_table);
	// 门前清（暗杠不影响）
	if (fixed_cnt == 0 || (pack_get_type(packs[3]) == PACK_TYPE_KONG && !is_pack_melded(packs[3]))) {
		if (win_flag & WIN_FLAG_SELF_DRAWN) {
			fan_table[FULLY_CONCEALED_HAND] = 1;
		}
		else {
			fan_table[CONCEALED_HAND] = 1;
		}
	}

	// 还原牌
	tile_t tiles[15];  // 第四组可能为杠，所以最多为15张
	memcpy(tiles, matched_seq, sizeof(*matched_seq));  // 组合龙的部分
	intptr_t tile_cnt = packs_to_tiles(&packs[3], 2, tiles + 9, 6);  // 一组面子+一对雀头 最多6张牌
	tile_cnt += 9;

	// 根据花色调整——涉及番种：无字、缺一门、混一色、清一色、五门齐
	adjust_by_suits(tiles, tile_cnt, fan_table);
	// 牌组以及牌特征就不需要调整了，有组合龙的存在绝对不可能存在全带幺、全带五、全双刻，断幺、推不倒、绿一色、字一色、清幺九、混幺九
	// 四归一调整
	adjust_by_tiles_hog(tiles, tile_cnt, fan_table);

	// 和牌张是组合龙范围的牌，不计边张、嵌张、单钓将
	if (std::none_of(std::begin(*matched_seq), std::end(*matched_seq), [win_tile](tile_t t) { return t == win_tile; })) {
		if (fixed_cnt == 0) {  // 门清的牌有可能存在边张、嵌张、单钓将
			// 将除去组合龙的部分恢复成牌
			--cnt_table[win_tile];
			tile_t temp[4];
			intptr_t cnt = table_to_tiles(cnt_table, temp, 4);

			// 根据听牌方式调整——涉及番种：边张、嵌张、单钓将
			adjust_by_waiting_form(packs + 3, 2, temp, cnt, win_tile, fan_table);
		}
		else {
			// 非门清状态如果听牌不在组合龙范围内，必然是单钓将
			fan_table[SINGLE_WAIT] = 1;
		}
	}

	// 统一调整一些不计的
	adjust_fan_table(fan_table);

	// 调整圈风刻、门风刻
	tile_t tile = pack_get_tile(packs[3]);
	if (is_winds(tile)) {
		adjust_by_winds(tile, prevalent_wind, seat_wind, fan_table);
	}

	return true;
}

// 十三幺
static FORCE_INLINE bool is_thirteen_orphans(const tile_t (&tiles)[14]) {
	return std::all_of(std::begin(tiles), std::end(tiles), &is_terminal_or_honor)
		&& std::includes(std::begin(tiles), std::end(tiles),
		std::begin(standard_thirteen_orphans), std::end(standard_thirteen_orphans));
}

// 全不靠/七星不靠算番
static bool calculate_honors_and_knitted_tiles(const tile_t (&standing_tiles)[14], fan_table_t &fan_table) {
	const tile_t *honor_begin = std::find_if(std::begin(standing_tiles), std::end(standing_tiles), &is_honor);
	int numbered_cnt = static_cast<int>(honor_begin - standing_tiles);
	// 数牌张数大于9或者小于7必然不可能是全不靠
	if (numbered_cnt > 9 || numbered_cnt < 7) {
		return false;
	}

	// 匹配组合龙
	if (std::none_of(&standard_knitted_straight[0], &standard_knitted_straight[6],
		[&standing_tiles, honor_begin](const tile_t (&seq)[9]) {
		return std::includes(std::begin(seq), std::end(seq), std::begin(standing_tiles), honor_begin);
	})) {
		return false;
	}

	if (numbered_cnt == 7 && std::equal(std::begin(standard_thirteen_orphans) + 6, std::end(standard_thirteen_orphans), standing_tiles + 7)) {
		// 七种字牌齐，为七星不靠
		fan_table[GREATER_HONORS_AND_KNITTED_TILES] = 1;
		return true;
	}
	else if (std::includes(std::begin(standard_thirteen_orphans) + 6, std::end(standard_thirteen_orphans), honor_begin, std::end(standing_tiles))) {
		// 全不靠
		fan_table[LESSER_HONORS_AND_KNITTED_TILES] = 1;
		if (numbered_cnt == 9) {  // 有9张数牌，为带组合龙的全不靠
			fan_table[KNITTED_STRAIGHT] = 1;
		}
		return true;
	}

	return false;
}

// 特殊和型算番
static bool calculate_special_form_fan(const tile_t (&standing_tiles)[14], win_flag_t win_flag, fan_table_t &fan_table) {
	// 七对
	if (standing_tiles[0] == standing_tiles[1]
		&& standing_tiles[2] == standing_tiles[3]
		&& standing_tiles[4] == standing_tiles[5]
		&& standing_tiles[6] == standing_tiles[7]
		&& standing_tiles[8] == standing_tiles[9]
		&& standing_tiles[10] == standing_tiles[11]
		&& standing_tiles[12] == standing_tiles[13]) {

		if (is_numbered_suit_quick(standing_tiles[0])
			&& standing_tiles[0] + 1 == standing_tiles[2]
			&& standing_tiles[2] + 1 == standing_tiles[4]
			&& standing_tiles[4] + 1 == standing_tiles[6]
			&& standing_tiles[6] + 1 == standing_tiles[8]
			&& standing_tiles[8] + 1 == standing_tiles[10]
			&& standing_tiles[10] + 1 == standing_tiles[12]) {
			// 连七对
			fan_table[SEVEN_SHIFTED_PAIRS] = 1;
			adjust_by_tiles_traits(standing_tiles, 14, fan_table);
		}
		else {
			// 普通七对
			fan_table[SEVEN_PAIRS] = 1;

			// 根据花色调整——涉及番种：无字、缺一门、混一色、清一色、五门齐
			adjust_by_suits(standing_tiles, 14, fan_table);
			// 根据牌特性调整——涉及番种：断幺、推不倒、绿一色、字一色、清幺九、混幺九
			adjust_by_tiles_traits(standing_tiles, 14, fan_table);
			// 根据数牌的范围调整——涉及番种：大于五、小于五、全大、全中、全小
			adjust_by_rank_range(standing_tiles, 14, fan_table);
			// 四归一调整
			adjust_by_tiles_hog(standing_tiles, 14, fan_table);
		}
	}
	// 十三幺
	else if (is_thirteen_orphans(standing_tiles)) {
		fan_table[THIRTEEN_ORPHANS] = 1;
	}
	// 全不靠/七星不靠
	else if (calculate_honors_and_knitted_tiles(standing_tiles, fan_table)) {
	}
	else {
		return false;
	}

	adjust_by_win_flag(win_flag, fan_table);
	// 统一调整一些不计的，根据风调整就没必要了，这些特殊和型都没有面子，不存在圈风刻、门风刻
	adjust_fan_table(fan_table);

	return true;
}

// 从番表计算番数
static int get_fan_by_table(const fan_table_t &fan_table) {
	int fan = 0;
	for (int i = 1; i < FAN_TABLE_SIZE; ++i) {
		if (fan_table[i] == 0) {
			continue;
		}
		fan += fan_value_table[i] * fan_table[i];
#if 0  // Debug
		if (fan_table[i] == 1) {
			LOG("%s %hu\n", fan_name[i], fan_value_table[i]);
		}
		else {
			LOG("%s %hu*%hu\n", fan_name[i], fan_value_table[i], fan_table[i]);
		}
#endif
	}
	return fan;
}

// 判断立牌是否包含和牌
bool is_standing_tiles_contains_win_tile(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t win_tile) {
	return std::any_of(standing_tiles, standing_tiles + standing_cnt,
		[win_tile](tile_t tile) { return tile == win_tile; });
}

// 统计和牌在副露牌组中出现的张数
size_t count_win_tile_in_fixed_packs(const pack_t *fixed_packs, intptr_t fixed_cnt, tile_t win_tile) {
	tile_table_t cnt_table = { 0 };
	for (intptr_t i = 0; i < fixed_cnt; ++i) {
		pack_t pack = fixed_packs[i];
		tile_t tile = pack_get_tile(pack);
		switch (pack_get_type(pack)) {
		case PACK_TYPE_CHOW: ++cnt_table[tile - 1]; ++cnt_table[tile]; ++cnt_table[tile + 1]; break;
		case PACK_TYPE_PUNG: cnt_table[tile] += 3; break;
		case PACK_TYPE_KONG: cnt_table[tile] += 4; break;
		default: break;
		}
	}
	return cnt_table[win_tile];
}

// 判断副露牌组是否包含杠
bool is_fixed_packs_contains_kong(const pack_t *fixed_packs, intptr_t fixed_cnt) {
	return std::any_of(fixed_packs, fixed_packs + fixed_cnt,
		[](pack_t pack) { return pack_get_type(pack) == PACK_TYPE_KONG; });
}

// 检查算番的输入是否合法
int check_calculator_input(const hand_tiles_t *hand_tiles, tile_t win_tile) {
	// 打表
	tile_table_t cnt_table;
	if (!map_hand_tiles(hand_tiles, &cnt_table)) {
		return ERROR_WRONG_TILES_COUNT;
	}
	if (win_tile != 0) {
		++cnt_table[win_tile];
	}

	// 如果某张牌超过4
	if (std::any_of(std::begin(cnt_table), std::end(cnt_table), [](int cnt) { return cnt > 4; })) {
		return ERROR_TILE_COUNT_GREATER_THAN_4;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 算番
//
int calculate_fan(const calculate_param_t *calculate_param, fan_table_t *fan_table) {
	const hand_tiles_t *hand_tiles = &calculate_param->hand_tiles;
	tile_t win_tile = calculate_param->win_tile;
	win_flag_t win_flag = calculate_param->win_flag;

	if (int ret = check_calculator_input(hand_tiles, win_tile)) {
		return ret;
	}

	intptr_t fixed_cnt = hand_tiles->pack_count;
	intptr_t standing_cnt = hand_tiles->tile_count;

	// 校正和牌标记
	// 如果立牌包含和牌，则必然不是和绝张
	const bool standing_tiles_contains_win_tile = is_standing_tiles_contains_win_tile(hand_tiles->standing_tiles, standing_cnt, win_tile);
	if (standing_tiles_contains_win_tile) {
		win_flag &= ~WIN_FLAG_4TH_TILE;
	}

	// 如果和牌在副露中出现3张，则必然为和绝张
	const size_t win_tile_in_fixed_packs = count_win_tile_in_fixed_packs(hand_tiles->fixed_packs, fixed_cnt, win_tile);
	if (3 == win_tile_in_fixed_packs) {
		win_flag |= WIN_FLAG_4TH_TILE;
	}

	// 附加杠标记
	if (win_flag & WIN_FLAG_ABOUT_KONG) {
		if (win_flag & WIN_FLAG_SELF_DRAWN) {  // 自摸
			// 如果手牌没有杠，则必然不是杠上开花
			if (!is_fixed_packs_contains_kong(hand_tiles->fixed_packs, fixed_cnt)) {
				win_flag &= ~WIN_FLAG_ABOUT_KONG;
			}
		}
		else {  // 点和
			// 如果和牌在手牌范围内出现过，则必然不是抢杠和
			if (win_tile_in_fixed_packs > 0 || standing_tiles_contains_win_tile) {
				win_flag &= ~WIN_FLAG_ABOUT_KONG;
			}
		}
	}

	// 合并立牌与和牌，并排序，最多为14张
	tile_t standing_tiles[14];
	memcpy(standing_tiles, hand_tiles->standing_tiles, standing_cnt * sizeof(tile_t));
	standing_tiles[standing_cnt] = win_tile;
	std::sort(standing_tiles, standing_tiles + standing_cnt + 1);

	// 最大番标记
	int max_fan = 0;
	const fan_table_t *selected_fan_table = nullptr;

	// 特殊和型的番
	fan_table_t special_fan_table = { 0 };

	// 先判断各种特殊和型
	if (fixed_cnt == 0) {  // 门清状态，有可能是基本和型组合龙
		if (calculate_knitted_straight_fan(calculate_param, win_flag, special_fan_table)) {
			max_fan = get_fan_by_table(special_fan_table);
			selected_fan_table = &special_fan_table;
			LOG("fan = %d\n\n", max_fan);
		}
		else if (calculate_special_form_fan(standing_tiles, win_flag, special_fan_table)) {
			max_fan = get_fan_by_table(special_fan_table);
			selected_fan_table = &special_fan_table;
			LOG("fan = %d\n\n", max_fan);
		}
	}
	else if (fixed_cnt == 1) {  // 1副露状态，有可能是基本和型组合龙
		if (calculate_knitted_straight_fan(calculate_param, win_flag, special_fan_table)) {
			max_fan = get_fan_by_table(special_fan_table);
			selected_fan_table = &special_fan_table;
			LOG("fan = %d\n\n", max_fan);
		}
	}

	// 无法构成特殊和型或者为七对
	// 七对也要按基本和型划分，因为极端情况下，基本和型的番会超过七对的番
	if (selected_fan_table == nullptr || special_fan_table[SEVEN_PAIRS] == 1) {
		// 划分
		division_result_t result;
		if (divide_win_hand(standing_tiles, hand_tiles->fixed_packs, fixed_cnt, &result)) {
			fan_table_t fan_tables[MAX_DIVISION_CNT] = { { 0 } };

			// 遍历各种划分方式，分别算番，找出最大的番的划分方式
			for (intptr_t i = 0; i < result.count; ++i) {
#if 0  // Debug
				char str[64];
				packs_to_string(result.divisions[i].packs, 5, str, sizeof(str));
				puts(str);
#endif
				calculate_basic_form_fan(result.divisions[i].packs, calculate_param, win_flag, fan_tables[i]);
				int current_fan = get_fan_by_table(fan_tables[i]);
				if (current_fan > max_fan) {
					max_fan = current_fan;
					selected_fan_table = &fan_tables[i];
				}
				LOG("fan = %d\n\n", current_fan);
			}
		}
	}

	if (selected_fan_table == nullptr) {
		return ERROR_NOT_WIN;
	}

	// 加花牌
	max_fan += calculate_param->flower_count;

	if (fan_table != nullptr) {
		memcpy(*fan_table, *selected_fan_table, sizeof(*fan_table));
		(*fan_table)[FLOWER_TILES] = calculate_param->flower_count;
	}

	return max_fan;
}

}

/*** End of inlined file: fan_calculator.cpp ***/


/*** Start of inlined file: shanten.cpp ***/
#include <assert.h>
#include <string.h>
#include <limits>
#include <algorithm>
#include <iterator>

namespace mahjong {

// 牌组转换成牌
intptr_t packs_to_tiles(const pack_t *packs, intptr_t pack_cnt, tile_t *tiles, intptr_t tile_cnt) {
	if (packs == nullptr || tiles == nullptr) {
		return 0;
	}

	intptr_t cnt = 0;
	for (int i = 0; i < pack_cnt && cnt < tile_cnt; ++i) {
		tile_t tile = pack_get_tile(packs[i]);
		switch (pack_get_type(packs[i])) {
		case PACK_TYPE_CHOW:
			if (cnt < tile_cnt) tiles[cnt++] = static_cast<tile_t>(tile - 1);
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			if (cnt < tile_cnt) tiles[cnt++] = static_cast<tile_t>(tile + 1);
			break;
		case PACK_TYPE_PUNG:
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			break;
		case PACK_TYPE_KONG:
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			break;
		case PACK_TYPE_PAIR:
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			if (cnt < tile_cnt) tiles[cnt++] = tile;
			break;
		default:
			UNREACHABLE();
			break;
		}
	}
	return cnt;
}

// 将牌打表
void map_tiles(const tile_t *tiles, intptr_t cnt, tile_table_t *cnt_table) {
	memset(*cnt_table, 0, sizeof(*cnt_table));
	for (intptr_t i = 0; i < cnt; ++i) {
		++(*cnt_table)[tiles[i]];
	}
}

// 将手牌打表
bool map_hand_tiles(const hand_tiles_t *hand_tiles, tile_table_t *cnt_table) {
	// 将每一组副露当作3张牌来算，那么总张数=13
	if (hand_tiles->tile_count <= 0 || hand_tiles->pack_count < 0 || hand_tiles->pack_count > 4
		|| hand_tiles->pack_count * 3 + hand_tiles->tile_count != 13) {
		return false;
	}

	// 将副露恢复成牌
	tile_t tiles[18];
	intptr_t tile_cnt = 0;
	if (hand_tiles->pack_count == 0) {
		memcpy(tiles, hand_tiles->standing_tiles, 13 * sizeof(tile_t));
		tile_cnt = 13;
	}
	else {
		tile_cnt = packs_to_tiles(hand_tiles->fixed_packs, hand_tiles->pack_count, tiles, 18);
		memcpy(tiles + tile_cnt, hand_tiles->standing_tiles, hand_tiles->tile_count * sizeof(tile_t));
		tile_cnt += hand_tiles->tile_count;
	}

	// 打表
	map_tiles(tiles, tile_cnt, cnt_table);
	return true;
}

// 将表转换成牌
intptr_t table_to_tiles(const tile_table_t &cnt_table, tile_t *tiles, intptr_t max_cnt) {
	intptr_t cnt = 0;
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		for (int n = 0; n < cnt_table[t]; ++n) {
			*tiles++ = t;
			++cnt;
			if (cnt == max_cnt) {
				return max_cnt;
			}
		}
	}
	return cnt;
}

namespace {

	// 路径单元，单元有面子、雀头、搭子等种类，见下面的宏
	// 高8位表示类型，低8位表示牌
	// 对于顺子和顺子搭子，牌指的是最小的一张牌，
	// 例如在顺子123万中，牌为1万，在两面搭子45条中，牌为4条等等
	typedef uint16_t path_unit_t;

#define UNIT_TYPE_CHOW 1                // 顺子
#define UNIT_TYPE_PUNG 2                // 刻子
#define UNIT_TYPE_PAIR 4                // 雀头
#define UNIT_TYPE_CHOW_OPEN_END 5       // 两面或者边张搭子
#define UNIT_TYPE_CHOW_CLOSED 6         // 嵌张搭子
#define UNIT_TYPE_INCOMPLETE_PUNG 7     // 刻子搭子

#define MAKE_UNIT(type_, tile_) static_cast<path_unit_t>(((type_) << 8) | (tile_))
#define UNIT_TYPE(unit_) (((unit_) >> 8) & 0xFF)
#define UNIT_TILE(unit_) ((unit_) & 0xFF)

#define MAX_STATE 512
#define UNIT_SIZE 7

	// 一条路径
	struct work_path_t {
		path_unit_t units[UNIT_SIZE];  // 14/2=7最多7个搭子
		uint16_t depth;  // 当前路径深度
	};

	// 当前工作状态
	struct work_state_t {
		work_path_t paths[MAX_STATE];  // 所有路径
		intptr_t count;  // 路径数量
	};
}

// 路径是否来过了
static bool is_basic_form_branch_exist(const intptr_t fixed_cnt, const work_path_t *work_path, const work_state_t *work_state) {
	if (work_state->count <= 0 || work_path->depth == 0) {
		return false;
	}

	// depth处有信息，所以按stl风格的end应该要+1
	const uint16_t depth = static_cast<uint16_t>(work_path->depth + 1);

	// std::includes要求有序，但又不能破坏当前数据
	work_path_t temp;
	std::copy(&work_path->units[fixed_cnt], &work_path->units[depth], &temp.units[fixed_cnt]);
	std::sort(&temp.units[fixed_cnt], &temp.units[depth]);

	return std::any_of(&work_state->paths[0], &work_state->paths[work_state->count],
		[&temp, fixed_cnt, depth](const work_path_t &path) {
		return std::includes(&path.units[fixed_cnt], &path.units[path.depth], &temp.units[fixed_cnt], &temp.units[depth]);
	});
}

// 保存路径
static void save_work_path(const intptr_t fixed_cnt, const work_path_t *work_path, work_state_t *work_state) {
	// 复制一份数据，不破坏当前数据
	work_path_t temp;
	temp.depth = work_path->depth;
	std::copy(&work_path->units[fixed_cnt], &work_path->units[temp.depth + 1], &temp.units[fixed_cnt]);
	std::sort(&temp.units[fixed_cnt], &temp.units[temp.depth + 1]);

	// 判断是否重复
	if (std::none_of(&work_state->paths[0], &work_state->paths[work_state->count],
		[&temp, fixed_cnt](const work_path_t &path) {
		return (path.depth == temp.depth && std::equal(&path.units[fixed_cnt], &path.units[path.depth + 1], &temp.units[fixed_cnt]));
	})) {
		if (work_state->count < MAX_STATE) {
			work_path_t &path = work_state->paths[work_state->count++];
			path.depth = temp.depth;
			std::copy(&temp.units[fixed_cnt], &temp.units[temp.depth + 1], &path.units[fixed_cnt]);
		}
		else {
			assert(0 && "too many state!");
		}
	}
}

// 递归计算基本和型上听数
// 参数说明：
//   cnt_table牌表
//   has_pair是否有雀头
//   pack_cnt完成的面子数
//   incomplete_cnt搭子数
// 最后三个参数为优化性能用的，
// work_path保存当前正在计算的路径，
// work_state保存了所有已经计算过的路径，
// 从0到fixed_cnt的数据是不使用的，这些保留给了副露的面子
static int basic_form_shanten_recursively(tile_table_t &cnt_table, const bool has_pair, const unsigned pack_cnt, const unsigned incomplete_cnt,
	const intptr_t fixed_cnt, work_path_t *work_path, work_state_t *work_state) {
	if (fixed_cnt == 4) {  // 4副露
		for (int i = 0; i < 34; ++i) {
			tile_t t = all_tiles[i];
			if (cnt_table[t] > 1) {
				return -1;
			}
		}
		return 0;
	}

	if (pack_cnt == 4) {  // 已经有4组面子
		return has_pair ? -1 : 0;  // 如果有雀头，则和了；如果无雀头，则是听牌
	}

	int max_ret;  // 当前状态能返回的最大上听数

	// 算法说明：
	// 缺少的面子数=4-完成的面子数
	// 缺少的搭子数=缺少的面子数-已有的搭子数
	// 两式合并：缺少的搭子数=4-完成的面子数-已有的搭子数
	int incomplete_need = 4 - pack_cnt - incomplete_cnt;
	if (incomplete_need > 0) {  // 还需要搭子的情况
		// 有雀头时，上听数=已有的搭子数+缺少的搭子数*2-1
		// 无雀头时，上听数=已有的搭子数+缺少的搭子数*2
		max_ret = incomplete_cnt + incomplete_need * 2 - (has_pair ? 1 : 0);
	}
	else {  // 搭子齐了的情况
		// 有雀头时，上听数=3-完成的面子数
		// 无雀头时，上听数=4-完成的面子数
		max_ret = (has_pair ? 3 : 4) - pack_cnt;
	}

	// 当前路径深度
	const unsigned depth = pack_cnt + incomplete_cnt + has_pair;
	work_path->depth = static_cast<uint16_t>(depth);

	int result = max_ret;

	if (pack_cnt + incomplete_cnt > 4) {  // 搭子超载
		save_work_path(fixed_cnt, work_path, work_state);
		return max_ret;
	}

	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] < 1) {
			continue;
		}

		// 雀头
		if (!has_pair && cnt_table[t] > 1) {
			work_path->units[depth] = MAKE_UNIT(UNIT_TYPE_PAIR, t);  // 记录雀头
			if (!is_basic_form_branch_exist(fixed_cnt, work_path, work_state)) {
				// 削减雀头，递归
				cnt_table[t] -= 2;
				int ret = basic_form_shanten_recursively(cnt_table, true, pack_cnt, incomplete_cnt,
					fixed_cnt, work_path, work_state);
				result = std::min(ret, result);
				// 还原
				cnt_table[t] += 2;
			}
		}

		// 刻子
		if (cnt_table[t] > 2) {
			work_path->units[depth] = MAKE_UNIT(UNIT_TYPE_PUNG, t);  // 记录刻子
			if (!is_basic_form_branch_exist(fixed_cnt, work_path, work_state)) {
				// 削减这组刻子，递归
				cnt_table[t] -= 3;
				int ret = basic_form_shanten_recursively(cnt_table, has_pair, pack_cnt + 1, incomplete_cnt,
					fixed_cnt, work_path, work_state);
				result = std::min(ret, result);
				// 还原
				cnt_table[t] += 3;
			}
		}

		// 顺子（只能是数牌）
		bool is_numbered = is_numbered_suit(t);
		// 顺子t t+1 t+2，显然t不能是8点以上的数牌
		if (is_numbered && tile_get_rank(t) < 8 && cnt_table[t + 1] && cnt_table[t + 2]) {
			work_path->units[depth] = MAKE_UNIT(UNIT_TYPE_CHOW, t);  // 记录顺子
			if (!is_basic_form_branch_exist(fixed_cnt, work_path, work_state)) {
				// 削减这组顺子，递归
				--cnt_table[t];
				--cnt_table[t + 1];
				--cnt_table[t + 2];
				int ret = basic_form_shanten_recursively(cnt_table, has_pair, pack_cnt + 1, incomplete_cnt,
					fixed_cnt, work_path, work_state);
				result = std::min(ret, result);
				// 还原
				++cnt_table[t];
				++cnt_table[t + 1];
				++cnt_table[t + 2];
			}
		}

		// 如果已经通过削减雀头/面子降低了上听数，再按搭子计算的上听数肯定不会更少
		if (result < max_ret) {
			continue;
		}

		// 刻子搭子
		if (cnt_table[t] > 1) {
			work_path->units[depth] = MAKE_UNIT(UNIT_TYPE_INCOMPLETE_PUNG, t);  // 记录刻子搭子
			if (!is_basic_form_branch_exist(fixed_cnt, work_path, work_state)) {
				// 削减刻子搭子，递归
				cnt_table[t] -= 2;
				int ret = basic_form_shanten_recursively(cnt_table, has_pair, pack_cnt, incomplete_cnt + 1,
					fixed_cnt, work_path, work_state);
				result = std::min(ret, result);
				// 还原
				cnt_table[t] += 2;
			}
		}

		// 顺子搭子（只能是数牌）
		if (is_numbered) {
			// 两面或者边张搭子t t+1，显然t不能是9点以上的数牌
			if (tile_get_rank(t) < 9 && cnt_table[t + 1]) {  // 两面或者边张
				work_path->units[depth] = MAKE_UNIT(UNIT_TYPE_CHOW_OPEN_END, t);  // 记录两面或者边张搭子
				if (!is_basic_form_branch_exist(fixed_cnt, work_path, work_state)) {
					// 削减搭子，递归
					--cnt_table[t];
					--cnt_table[t + 1];
					int ret = basic_form_shanten_recursively(cnt_table, has_pair, pack_cnt, incomplete_cnt + 1,
						fixed_cnt, work_path, work_state);
					result = std::min(ret, result);
					// 还原
					++cnt_table[t];
					++cnt_table[t + 1];
				}
			}
			// 嵌张搭子t t+2，显然t不能是8点以上的数牌
			if (tile_get_rank(t) < 8 && cnt_table[t + 2]) {  // 嵌张
				work_path->units[depth] = MAKE_UNIT(UNIT_TYPE_CHOW_CLOSED, t);  // 记录嵌张搭子
				if (!is_basic_form_branch_exist(fixed_cnt, work_path, work_state)) {
					// 削减搭子，递归
					--cnt_table[t];
					--cnt_table[t + 2];
					int ret = basic_form_shanten_recursively(cnt_table, has_pair, pack_cnt, incomplete_cnt + 1,
						fixed_cnt, work_path, work_state);
					result = std::min(ret, result);
					// 还原
					++cnt_table[t];
					++cnt_table[t + 2];
				}
			}
		}
	}

	if (result == max_ret) {
		save_work_path(fixed_cnt, work_path, work_state);
	}

	return result;
}

// 数牌是否有搭子
static bool numbered_tile_has_neighbor(const tile_table_t &cnt_table, tile_t t) {
	rank_t r = tile_get_rank(t);
	if (r < 9) { if (cnt_table[t + 1]) return true; }
	if (r < 8) { if (cnt_table[t + 2]) return true; }
	if (r > 1) { if (cnt_table[t - 1]) return true; }
	if (r > 2) { if (cnt_table[t - 2]) return true; }
	return false;
}

// 以表格为参数计算基本和型上听数
static int basic_form_shanten_from_table(tile_table_t &cnt_table, intptr_t fixed_cnt, useful_table_t *useful_table) {
	// 计算上听数
	work_path_t work_path;
	work_state_t work_state;
	work_state.count = 0;
	int result = basic_form_shanten_recursively(cnt_table, false, static_cast<uint16_t>(fixed_cnt), 0,
		fixed_cnt, &work_path, &work_state);

	if (useful_table == nullptr) {
		return result;
	}

	// 穷举所有的牌，获取能减少上听数的牌
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] == 4 && result > 0) {
			continue;
		}

		if (cnt_table[t] == 0) {
			// 跳过孤张字牌和不靠张的数牌，这些牌都无法减少上听数
			if (is_honor(t) || !numbered_tile_has_neighbor(cnt_table, t)) {
				continue;
			}
		}

		++cnt_table[t];
		work_state.count = 0;
		int temp = basic_form_shanten_recursively(cnt_table, false, static_cast<uint16_t>(fixed_cnt), 0,
			fixed_cnt, &work_path, &work_state);
		if (temp < result) {
			(*useful_table)[t] = true;  // 标记为有效牌
		}
		--cnt_table[t];
	}

	return result;
}

// 基本和型上听数
int basic_form_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table) {
	if (standing_tiles == nullptr || (standing_cnt != 13
		&& standing_cnt != 10 && standing_cnt != 7 && standing_cnt != 4 && standing_cnt != 1)) {
		return std::numeric_limits<int>::max();
	}

	// 对立牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);

	if (useful_table != nullptr) {
		memset(*useful_table, 0, sizeof(*useful_table));
	}
	return basic_form_shanten_from_table(cnt_table, (13 - standing_cnt) / 3, useful_table);
}

// 基本和型判断1张是否听牌
static bool is_basic_form_wait_1(tile_table_t &cnt_table, useful_table_t *waiting_table) {
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] != 1) {
			continue;
		}

		// 单钓将
		cnt_table[t] = 0;
		if (std::all_of(std::begin(cnt_table), std::end(cnt_table), [](int n) { return n == 0; })) {
			cnt_table[t] = 1;
			if (waiting_table != nullptr) {  // 不需要获取听牌张，则可以直接返回
				(*waiting_table)[t] = true;
			}
			return true;
		}
		cnt_table[t] = 1;
	}

	return false;
}

// 基本和型判断2张是否听牌
static bool is_basic_form_wait_2(const tile_table_t &cnt_table, useful_table_t *waiting_table) {
	bool ret = false;
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] < 1) {
			continue;
		}
		if (cnt_table[t] > 1) {
			if (waiting_table != nullptr) {  // 获取听牌张
				(*waiting_table)[t] = true;  // 对倒
				ret = true;
				continue;
			}
			else {  // 不需要获取听牌张，则可以直接返回
				return true;
			}
		}
		if (is_numbered_suit_quick(t)) {  // 数牌搭子
			rank_t r = tile_get_rank(t);
			if (r > 1 && cnt_table[t - 1]) {  // 两面或者边张
				if (waiting_table != nullptr) {  // 获取听牌张
					if (r < 9) (*waiting_table)[t + 1] = true;
					if (r > 2) (*waiting_table)[t - 2] = true;
					ret = true;
					continue;
				}
				else {  // 不需要获取听牌张，则可以直接返回
					return true;
				}
			}
			if (r > 2 && cnt_table[t - 2]) {  // 嵌张
				if (waiting_table != nullptr) {  // 获取听牌张
					(*waiting_table)[t - 1] = true;
					ret = true;
					continue;
				}
				else {  // 不需要获取听牌张，则可以直接返回
					return true;
				}
			}
		}
	}
	return ret;
}

// 基本和型判断4张是否听牌
static bool is_basic_form_wait_4(tile_table_t &cnt_table, useful_table_t *waiting_table) {
	bool ret = false;
	// 削减雀头
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] < 2) {
			continue;
		}
		// 削减雀头，递归
		cnt_table[t] -= 2;
		if (is_basic_form_wait_2(cnt_table, waiting_table)) {
			ret = true;
		}
		// 还原
		cnt_table[t] += 2;
		if (ret && waiting_table == nullptr) {  // 不需要获取听牌张，则可以直接结束递归
			return true;
		}
	}

	return ret;
}

// 递归计算基本和型是否听牌
static bool is_basic_form_wait_recursively(tile_table_t &cnt_table, intptr_t left_cnt, useful_table_t *waiting_table) {
	if (left_cnt == 1) {
		return is_basic_form_wait_1(cnt_table, waiting_table);
	}

	bool ret = false;
	if (left_cnt == 4) {
		ret = is_basic_form_wait_4(cnt_table, waiting_table);
		if (ret && waiting_table == nullptr) {  // 不需要获取听牌张，则可以直接结束递归
			return true;
		}
	}

	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] < 1) {
			continue;
		}

		// 刻子
		if (cnt_table[t] > 2) {
			// 削减这组刻子，递归
			cnt_table[t] -= 3;
			if (is_basic_form_wait_recursively(cnt_table, left_cnt - 3, waiting_table)) {
				ret = true;
			}
			// 还原
			cnt_table[t] += 3;
			if (ret && waiting_table == nullptr) {  // 不需要获取听牌张，则可以直接结束递归
				return true;
			}
		}

		// 顺子（只能是数牌）
		if (is_numbered_suit(t)) {
			// 顺子t t+1 t+2，显然t不能是8点以上的数牌
			if (tile_get_rank(t) < 8 && cnt_table[t + 1] && cnt_table[t + 2]) {
				// 削减这组顺子，递归
				--cnt_table[t];
				--cnt_table[t + 1];
				--cnt_table[t + 2];
				if (is_basic_form_wait_recursively(cnt_table, left_cnt - 3, waiting_table)) {
					ret = true;
				}
				// 还原
				++cnt_table[t];
				++cnt_table[t + 1];
				++cnt_table[t + 2];
				if (ret && waiting_table == nullptr) {  // 不需要获取听牌张，则可以直接结束递归
					return true;
				}
			}
		}
	}

	return ret;
}

// 基本和型是否听牌
// 这里之所以不用直接调用上听数计算函数，判断其返回值为0的方式
// 是因为前者会削减搭子，这个操作在和牌判断中是没必要的，所以单独写一套更快逻辑
bool is_basic_form_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table) {
	// 对立牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);

	if (waiting_table != nullptr) {
		memset(*waiting_table, 0, sizeof(*waiting_table));
	}
	return is_basic_form_wait_recursively(cnt_table, standing_cnt, waiting_table);
}

// 基本和型2张能否和牌
static bool is_basic_form_win_2(const tile_table_t &cnt_table) {
	// 找到未使用的牌
	typedef std::remove_all_extents<tile_table_t>::type table_elem_t;
	const table_elem_t *it = std::find_if(std::begin(cnt_table), std::end(cnt_table), [](table_elem_t n) { return n > 0; });
	// 存在且张数等于2
	if (it == std::end(cnt_table) || *it != 2) {
		return false;
	}
	// 还有其他未使用的牌
	return std::none_of(it + 1, std::end(cnt_table), [](int n) { return n > 0; });
}

// 递归计算基本和型是否和牌
// 这里之所以不用直接调用上听数计算函数，判断其返回值为-1的方式，
// 是因为前者会削减搭子，这个操作在和牌判断中是没必要的，所以单独写一套更快逻辑
static bool is_basic_form_win_recursively(tile_table_t &cnt_table, intptr_t left_cnt) {
	if (left_cnt == 2) {
		return is_basic_form_win_2(cnt_table);
	}

	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] < 1) {
			continue;
		}

		// 刻子
		if (cnt_table[t] > 2) {
			// 削减这组刻子，递归
			cnt_table[t] -= 3;
			bool ret = is_basic_form_win_recursively(cnt_table, left_cnt - 3);
			// 还原
			cnt_table[t] += 3;
			if (ret) {
				return true;
			}
		}

		// 顺子（只能是数牌）
		if (is_numbered_suit(t)) {
			// 顺子t t+1 t+2，显然t不能是8点以上的数牌
			if (tile_get_rank(t) < 8 && cnt_table[t + 1] && cnt_table[t + 2]) {
				// 削减这组顺子，递归
				--cnt_table[t];
				--cnt_table[t + 1];
				--cnt_table[t + 2];
				bool ret = is_basic_form_win_recursively(cnt_table, left_cnt - 3);
				// 还原
				++cnt_table[t];
				++cnt_table[t + 1];
				++cnt_table[t + 2];
				if (ret) {
					return true;
				}
			}
		}
	}

	return false;
}

// 基本和型是否和牌
bool is_basic_form_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile) {
	// 对立牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);
	++cnt_table[test_tile];  // 添加测试的牌
	return is_basic_form_win_recursively(cnt_table, standing_cnt + 1);
}

//-------------------------------- 七对 --------------------------------

// 七对上听数
int seven_pairs_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table) {
	if (standing_tiles == nullptr || standing_cnt != 13) {
		return std::numeric_limits<int>::max();
	}

	// 对牌的种类进行打表，并统计对子数
	int pair_cnt = 0;
	tile_table_t cnt_table = { 0 };
	for (intptr_t i = 0; i < standing_cnt; ++i) {
		tile_t tile = standing_tiles[i];
		++cnt_table[tile];
		if (cnt_table[tile] == 2) {
			++pair_cnt;
			cnt_table[tile] = 0;
		}
	}

	// 有效牌
	if (useful_table != nullptr) {
		std::transform(std::begin(cnt_table), std::end(cnt_table), std::begin(*useful_table), [](int n) { return n != 0; });
	}
	return 6 - pair_cnt;
}

// 七对是否听牌
bool is_seven_pairs_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table) {
	// 直接计算其上听数，上听数为0即为听牌
	if (waiting_table == nullptr) {
		return (0 == seven_pairs_shanten(standing_tiles, standing_cnt, nullptr));
	}

	useful_table_t useful_table;
	if (0 == seven_pairs_shanten(standing_tiles, standing_cnt, &useful_table)) {
		memcpy(*waiting_table, useful_table, sizeof(*waiting_table));
		return true;
	}
	return false;
}

// 七对是否和牌
bool is_seven_pairs_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile) {
	useful_table_t useful_table;
	return (0 == seven_pairs_shanten(standing_tiles, standing_cnt, &useful_table)
		&& useful_table[test_tile]);
}

//-------------------------------- 十三幺 --------------------------------

// 十三幺上听数
int thirteen_orphans_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table) {
	if (standing_tiles == nullptr || standing_cnt != 13) {
		return std::numeric_limits<int>::max();
	}

	// 对牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);

	bool has_pair = false;
	int cnt = 0;
	for (int i = 0; i < 13; ++i) {
		int n = cnt_table[standard_thirteen_orphans[i]];
		if (n > 0) {
			++cnt;  // 幺九牌的种类
			if (n > 1) {
				has_pair = true;  // 幺九牌对子
			}
		}
	}

	// 当有对子时，上听数为：12-幺九牌的种类
	// 当没有对子时，上听数为：13-幺九牌的种类
	int ret = has_pair ? 12 - cnt : 13 - cnt;

	if (useful_table != nullptr) {
		// 先标记所有的幺九牌为有效牌
		memset(*useful_table, 0, sizeof(*useful_table));
		std::for_each(std::begin(standard_thirteen_orphans), std::end(standard_thirteen_orphans),
			[useful_table](tile_t t) {
			(*useful_table)[t] = true;
		});

		// 当有对子时，已有的幺九牌都不需要了
		if (has_pair) {
			for (int i = 0; i < 13; ++i) {
				tile_t t = standard_thirteen_orphans[i];
				int n = cnt_table[t];
				if (n > 0) {
					(*useful_table)[t] = false;
				}
			}
		}
	}

	return ret;
}

// 十三幺是否听牌
bool is_thirteen_orphans_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table) {
	// 直接计算其上听数，上听数为0即为听牌
	if (waiting_table == nullptr) {
		return (0 == thirteen_orphans_shanten(standing_tiles, standing_cnt, nullptr));
	}

	useful_table_t useful_table;
	if (0 == thirteen_orphans_shanten(standing_tiles, standing_cnt, &useful_table)) {
		memcpy(*waiting_table, useful_table, sizeof(*waiting_table));
		return true;
	}
	return false;
}

// 十三幺是否和牌
bool is_thirteen_orphans_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile) {
	useful_table_t useful_table;
	return (0 == thirteen_orphans_shanten(standing_tiles, standing_cnt, &useful_table)
		&& useful_table[test_tile]);
}

//-------------------------------- “组合龙+面子+雀头”和型 --------------------------------

// 以表格为参数计算组合龙是否听牌
static bool is_knitted_straight_wait_from_table(const tile_table_t &cnt_table, intptr_t left_cnt, useful_table_t *waiting_table) {
	// 匹配组合龙
	const tile_t (*matched_seq)[9] = nullptr;
	tile_t missing_tiles[9];
	int missing_cnt = 0;
	for (int i = 0; i < 6; ++i) {  // 逐个组合龙测试
		missing_cnt = 0;
		for (int k = 0; k < 9; ++k) {
			tile_t t = standard_knitted_straight[i][k];
			if (cnt_table[t] == 0) {  // 缺失的
				missing_tiles[missing_cnt++] = t;
			}
		}
		if (missing_cnt < 2) {  // 缺2张或以上的肯定没听
			matched_seq = &standard_knitted_straight[i];
			break;
		}
	}

	if (matched_seq == nullptr || missing_cnt > 2) {
		return false;
	}

	if (waiting_table != nullptr) {
		memset(*waiting_table, 0, sizeof(*waiting_table));
	}

	// 剔除组合龙
	tile_table_t temp_table;
	memcpy(&temp_table, &cnt_table, sizeof(temp_table));
	for (int i = 0; i < 9; ++i) {
		tile_t t = (*matched_seq)[i];
		if (temp_table[t]) {
			--temp_table[t];
		}
	}

	if (missing_cnt == 1) {  // 如果缺一张，那么除去组合龙之后的牌应该是完成状态才能听牌
		if (left_cnt == 10) {
			if (is_basic_form_win_recursively(temp_table, 2)) {
				if (waiting_table != nullptr) {  // 获取听牌张，听组合龙缺的一张
					(*waiting_table)[missing_tiles[0]] = true;
				}
				return true;
			}
		}
		else {
			if (is_basic_form_win_recursively(temp_table, 5)) {
				if (waiting_table != nullptr) {  // 获取听牌张，听组合龙缺的一张
					(*waiting_table)[missing_tiles[0]] = true;
				}
				return true;
			}
		}
	}
	else if (missing_cnt == 0) {  // 如果组合龙齐了，那么除去组合龙之后的牌要能听，整手牌才能听
		if (left_cnt == 10) {
			return is_basic_form_wait_1(temp_table, waiting_table);
		}
		else {
			return is_basic_form_wait_recursively(temp_table, 4, waiting_table);
		}
	}

	return false;
}

// 基本和型包含主番的上听数，可用于计算三步高 三同顺 龙等三组面子的番种整个立牌的上听数
static int basic_form_shanten_specified(const tile_table_t &cnt_table, const tile_t *main_tiles, int main_cnt,
	intptr_t fixed_cnt, useful_table_t *useful_table) {

	tile_table_t temp_table;
	memcpy(&temp_table, &cnt_table, sizeof(temp_table));
	int exist_cnt = 0;

	// 统计主番的牌
	for (int i = 0; i < main_cnt; ++i) {
		tile_t t = main_tiles[i];
		int n = cnt_table[t];
		if (n > 0) {  // 有，削减之
			++exist_cnt;
			--temp_table[t];
		}
	}

	// 记录有效牌
	if (useful_table != nullptr) {
		memset(*useful_table, 0, sizeof(*useful_table));

		// 统计主番缺失的牌
		for (int i = 0; i < main_cnt; ++i) {
			tile_t t = main_tiles[i];
			int n = cnt_table[t];
			if (n <= 0) {
				(*useful_table)[t] = true;
			}
		}
	}

	// 余下牌的上听数
	int result = basic_form_shanten_from_table(temp_table, fixed_cnt + main_cnt / 3, useful_table);

	// 上听数=主番缺少的张数+余下牌的上听数
	return (main_cnt - exist_cnt) + result;
}

// 组合龙上听数
int knitted_straight_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table) {
	if (standing_tiles == nullptr || (standing_cnt != 13 && standing_cnt != 10)) {
		return std::numeric_limits<int>::max();
	}

	// 打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);

	int ret = std::numeric_limits<int>::max();

	// 需要获取有效牌时，计算上听数的同时就获取有效牌了
	if (useful_table != nullptr) {
		memset(*useful_table, 0, sizeof(*useful_table));

		useful_table_t temp_table;

		// 6种组合龙分别计算
		for (int i = 0; i < 6; ++i) {
			int fixed_cnt = (13 - static_cast<int>(standing_cnt)) / 3;
			int st = basic_form_shanten_specified(cnt_table, standard_knitted_straight[i], 9, fixed_cnt, &temp_table);
			if (st < ret) {  // 上听数小的，直接覆盖数据
				ret = st;
				memcpy(*useful_table, temp_table, sizeof(*useful_table));  // 直接覆盖原来的有效牌数据
			}
			else if (st == ret) {  // 两种不同组合龙上听数如果相等的话，直接合并有效牌
				std::transform(std::begin(*useful_table), std::end(*useful_table), std::begin(temp_table),
					std::begin(*useful_table), [](bool u, bool t) { return u || t; });
			}
		}
	}
	else {
		// 6种组合龙分别计算
		for (int i = 0; i < 6; ++i) {
			int fixed_cnt = (13 - static_cast<int>(standing_cnt)) / 3;
			int st = basic_form_shanten_specified(cnt_table, standard_knitted_straight[i], 9, fixed_cnt, nullptr);
			if (st < ret) {
				ret = st;
			}
		}
	}

	return ret;
}

// 组合龙是否听牌
bool is_knitted_straight_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table) {
	if (standing_tiles == nullptr || (standing_cnt != 13 && standing_cnt != 10)) {
		return false;
	}

	// 对立牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);

	return is_knitted_straight_wait_from_table(cnt_table, standing_cnt, waiting_table);
}

// 组合龙是否和牌
bool is_knitted_straight_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile) {
	useful_table_t waiting_table;
	return (is_knitted_straight_wait(standing_tiles, standing_cnt, &waiting_table)
		&& waiting_table[test_tile]);
}

//-------------------------------- 全不靠/七星不靠 --------------------------------

// 1种组合龙的全不靠上听数
static int honors_and_knitted_tiles_shanten_1(const tile_t *standing_tiles, intptr_t standing_cnt, int which_seq, useful_table_t *useful_table) {
	if (standing_tiles == nullptr || standing_cnt != 13) {
		return std::numeric_limits<int>::max();
	}

	// 对牌的种类进行打表
	tile_table_t cnt_table;
	map_tiles(standing_tiles, standing_cnt, &cnt_table);

	int cnt = 0;

	// 统计组合龙部分的数牌
	for (int i = 0; i < 9; ++i) {
		tile_t t = standard_knitted_straight[which_seq][i];
		int n = cnt_table[t];
		if (n > 0) {  // 有，增加计数
			++cnt;
		}
	}

	// 统计字牌
	for (int i = 6; i < 13; ++i) {
		tile_t t = standard_thirteen_orphans[i];
		int n = cnt_table[t];
		if (n > 0) {  // 有，增加计数
			++cnt;
		}
	}

	// 记录有效牌
	if (useful_table != nullptr) {
		memset(*useful_table, 0, sizeof(*useful_table));

		// 统计组合龙部分缺失的数牌
		for (int i = 0; i < 9; ++i) {
			tile_t t = standard_knitted_straight[which_seq][i];
			int n = cnt_table[t];
			if (n <= 0) {
				(*useful_table)[t] = true;
			}
		}

		// 统计缺失的字牌
		for (int i = 6; i < 13; ++i) {
			tile_t t = standard_thirteen_orphans[i];
			int n = cnt_table[t];
			if (n <= 0) {
				(*useful_table)[t] = true;
			}
		}
	}

	// 上听数=13-符合牌型的计数
	return 13 - cnt;
}

// 全不靠上听数
int honors_and_knitted_tiles_shanten(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *useful_table) {
	int ret = std::numeric_limits<int>::max();

	// 需要获取有效牌时，计算上听数的同时就获取有效牌了
	if (useful_table != nullptr) {
		memset(*useful_table, 0, sizeof(*useful_table));

		useful_table_t temp_table;

		// 6种组合龙分别计算
		for (int i = 0; i < 6; ++i) {
			int st = honors_and_knitted_tiles_shanten_1(standing_tiles, standing_cnt, i, &temp_table);
			if (st < ret) {  // 上听数小的，直接覆盖数据
				ret = st;
				memcpy(*useful_table, temp_table, sizeof(*useful_table));  // 直接覆盖原来的有效牌数据
			}
			else if (st == ret) {  // 两种不同组合龙上听数如果相等的话，直接合并有效牌
				std::transform(std::begin(*useful_table), std::end(*useful_table), std::begin(temp_table),
					std::begin(*useful_table), [](bool u, bool t) { return u || t; });
			}
		}
	}
	else {
		// 6种组合龙分别计算
		for (int i = 0; i < 6; ++i) {
			int st = honors_and_knitted_tiles_shanten_1(standing_tiles, standing_cnt, i, nullptr);
			if (st < ret) {
				ret = st;
			}
		}
	}
	return ret;
}

// 全不靠是否听牌
bool is_honors_and_knitted_tiles_wait(const tile_t *standing_tiles, intptr_t standing_cnt, useful_table_t *waiting_table) {
	// 直接计算其上听数，上听数为0即为听牌
	if (waiting_table == nullptr) {
		return (0 == honors_and_knitted_tiles_shanten(standing_tiles, standing_cnt, nullptr));
	}

	useful_table_t useful_table;
	if (0 == honors_and_knitted_tiles_shanten(standing_tiles, standing_cnt, &useful_table)) {
		memcpy(*waiting_table, useful_table, sizeof(*waiting_table));
		return true;
	}
	return false;
}

// 全不靠是否和牌
bool is_honors_and_knitted_tiles_win(const tile_t *standing_tiles, intptr_t standing_cnt, tile_t test_tile) {
	useful_table_t useful_table;
	if (0 == honors_and_knitted_tiles_shanten(standing_tiles, standing_cnt, &useful_table)) {
		return useful_table[test_tile];
	}
	return false;
}

//-------------------------------- 所有情况综合 --------------------------------

bool is_waiting(const hand_tiles_t &hand_tiles, useful_table_t *useful_table) {
	bool spcial_waiting = false, basic_waiting = false;
	useful_table_t table_special, table_basic;

	if (hand_tiles.tile_count == 13) {
		if (is_thirteen_orphans_wait(hand_tiles.standing_tiles, 13, &table_special)) {
			spcial_waiting = true;
		}
		else if (is_honors_and_knitted_tiles_wait(hand_tiles.standing_tiles, 13, &table_special)) {
			spcial_waiting = true;
		}
		else if (is_seven_pairs_wait(hand_tiles.standing_tiles, 13, &table_special)) {
			spcial_waiting = true;
		}
		else if (is_knitted_straight_wait(hand_tiles.standing_tiles, 13, &table_special)) {
			spcial_waiting = true;
		}
	}
	else if (hand_tiles.tile_count == 10) {
		if (is_knitted_straight_wait(hand_tiles.standing_tiles, 10, &table_special)) {
			spcial_waiting = true;
		}
	}

	if (is_basic_form_wait(hand_tiles.standing_tiles, hand_tiles.tile_count, &table_basic)) {
		basic_waiting = true;
	}

	if (useful_table != nullptr) {
		if (spcial_waiting && basic_waiting) {
			std::transform(std::begin(table_special), std::end(table_special), std::begin(table_basic), std::begin(*useful_table),
				[](bool a, bool b) { return a || b; });
		}
		else if (basic_waiting) {
			memcpy(*useful_table, table_basic, sizeof(table_basic));
		}
		else if (spcial_waiting) {
			memcpy(*useful_table, table_special, sizeof(table_special));
		}
	}

	return (spcial_waiting || basic_waiting);
}

//-------------------------------- 枚举打牌 --------------------------------

// 枚举打哪张牌1次
static bool enum_discard_tile_1(const hand_tiles_t *hand_tiles, tile_t discard_tile, uint8_t form_flag,
	void *context, enum_callback_t enum_callback) {
	enum_result_t result;
	result.discard_tile = discard_tile;
	result.form_flag = FORM_FLAG_BASIC_FORM;
	result.shanten = basic_form_shanten(hand_tiles->standing_tiles, hand_tiles->tile_count, &result.useful_table);
	if (result.shanten == 0 && result.useful_table[discard_tile]) {  // 0上听，并且打出的牌是有效牌，则修正为和了
		result.shanten = -1;
	}
	if (!enum_callback(context, &result)) {
		return false;
	}

	// 立牌有13张时，才需要计算特殊和型
	if (hand_tiles->tile_count == 13) {
		if (form_flag | FORM_FLAG_SEVEN_PAIRS) {
			result.form_flag = FORM_FLAG_SEVEN_PAIRS;
			result.shanten = seven_pairs_shanten(hand_tiles->standing_tiles, hand_tiles->tile_count, &result.useful_table);
			if (result.shanten == 0 && result.useful_table[discard_tile]) {  // 0上听，并且打出的牌是有效牌，则修正为和了
				result.shanten = -1;
			}
			if (!enum_callback(context, &result)) {
				return false;
			}
		}

		if (form_flag | FORM_FLAG_THIRTEEN_ORPHANS) {
			result.form_flag = FORM_FLAG_THIRTEEN_ORPHANS;
			result.shanten = thirteen_orphans_shanten(hand_tiles->standing_tiles, hand_tiles->tile_count, &result.useful_table);
			if (result.shanten == 0 && result.useful_table[discard_tile]) {  // 0上听，并且打出的牌是有效牌，则修正为和了
				result.shanten = -1;
			}
			if (!enum_callback(context, &result)) {
				return false;
			}
		}

		if (form_flag | FORM_FLAG_HONORS_AND_KNITTED_TILES) {
			result.form_flag = FORM_FLAG_HONORS_AND_KNITTED_TILES;
			result.shanten = honors_and_knitted_tiles_shanten(hand_tiles->standing_tiles, hand_tiles->tile_count, &result.useful_table);
			if (result.shanten == 0 && result.useful_table[discard_tile]) {  // 0上听，并且打出的牌是有效牌，则修正为和了
				result.shanten = -1;
			}
			if (!enum_callback(context, &result)) {
				return false;
			}
		}
	}

	// 立牌有13张或者10张时，才需要计算组合龙
	if (hand_tiles->tile_count == 13 || hand_tiles->tile_count == 10) {
		if (form_flag | FORM_FLAG_KNITTED_STRAIGHT) {
			result.form_flag = FORM_FLAG_KNITTED_STRAIGHT;
			result.shanten = knitted_straight_shanten(hand_tiles->standing_tiles, hand_tiles->tile_count, &result.useful_table);
			if (result.shanten == 0 && result.useful_table[discard_tile]) {  // 0上听，并且打出的牌是有效牌，则修正为和了
				result.shanten = -1;
			}
			if (!enum_callback(context, &result)) {
				return false;
			}
		}
	}

	return true;
}

// 枚举打哪张牌
void enum_discard_tile(const hand_tiles_t *hand_tiles, tile_t serving_tile, uint8_t form_flag,
	void *context, enum_callback_t enum_callback) {
	// 先计算摸切的
	if (!enum_discard_tile_1(hand_tiles, serving_tile, form_flag, context, enum_callback)) {
		return;
	}

	if (serving_tile == 0) {
		return;
	}

	// 将立牌打表
	tile_table_t cnt_table;
	map_tiles(hand_tiles->standing_tiles, hand_tiles->tile_count, &cnt_table);

	// 复制一份手牌
	hand_tiles_t temp;
	memcpy(&temp, hand_tiles, sizeof(temp));

	// 依次尝试打手中的立牌
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (cnt_table[t] && t != serving_tile && cnt_table[serving_tile] < 4) {
			--cnt_table[t];  // 打这张牌
			++cnt_table[serving_tile];  // 上这张牌

			// 从table转成立牌
			table_to_tiles(cnt_table, temp.standing_tiles, temp.tile_count);

			// 计算
			if (!enum_discard_tile_1(&temp, t, form_flag, context, enum_callback)) {
				return;
			}

			// 复原
			--cnt_table[serving_tile];
			++cnt_table[t];
		}
	}
}

}

/*** End of inlined file: shanten.cpp ***/

//Dangerous

using namespace std;

void MahjongInit();

vector<pair<int, string> > MahjongFanCalculator(
	vector<pair<string, pair<string, int> > > pack,
	vector<string> hand,
	string winTile,
	int flowerCount,
	bool isZIMO,
	bool isJUEZHANG,
	bool isGANG,
	bool isLAST,
	int menFeng,
	int quanFeng);

#endif

/*** End of inlined file: MahjongGB.h ***/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <assert.h>
#include <time.h>

using namespace mahjong;
using namespace std;
//#include "MahjongGB/MahjongGB.h"
#ifdef _BOTZONE_ONLINE

/*** Start of inlined file: json.h ***/
/// Json-cpp amalgated header (http://jsoncpp.sourceforge.net/).
/// It is intented to be used with #include <json/json.h>

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////

/*
The JsonCpp library's source code, including accompanying documentation,
tests and demonstration applications, are licensed under the following
conditions...

The author (Baptiste Lepilleur) explicitly disclaims copyright in all
jurisdictions which recognize such a disclaimer. In such jurisdictions,
this software is released into the Public Domain.

In jurisdictions which do not recognize Public Domain property (e.g. Germany as of
2010), this software is Copyright (c) 2007-2010 by Baptiste Lepilleur, and is
released under the terms of the MIT License (see below).

In jurisdictions which recognize Public Domain property, the user of this
software may choose to accept it either as 1) Public Domain, 2) under the
conditions of the MIT License (see below), or 3) under the terms of dual
Public Domain/MIT License conditions described here, as they choose.

The MIT License is about as close to Public Domain as a license can get, and is
described in clear, concise terms at:

   http://en.wikipedia.org/wiki/MIT_License

The full text of the MIT License follows:

========================================================================
Copyright (c) 2007-2010 Baptiste Lepilleur

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
========================================================================
(END LICENSE TEXT)

The MIT license is compatible with both the GPL and commercial
software, affording one all of the rights of Public Domain with the
minor nuisance of being required to keep the above copyright notice
and license text in the source code. Note also that by accepting the
Public Domain "license" you can re-license your copy using whatever
license you like.

*/

// //////////////////////////////////////////////////////////////////////
// End of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////

#ifndef JSON_AMALGATED_H_INCLUDED
# define JSON_AMALGATED_H_INCLUDED
/// If defined, indicates that the source file is amalgated
/// to prevent private header inclusion.
#define JSON_IS_AMALGAMATION

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/version.h
// //////////////////////////////////////////////////////////////////////

// DO NOT EDIT. This file is generated by CMake from  "version"
// and "version.h.in" files.
// Run CMake configure step to update it.
#ifndef JSON_VERSION_H_INCLUDED
# define JSON_VERSION_H_INCLUDED

# define JSONCPP_VERSION_STRING "0.7.0"
# define JSONCPP_VERSION_MAJOR 0
# define JSONCPP_VERSION_MINOR 7
# define JSONCPP_VERSION_PATCH 0
# define JSONCPP_VERSION_QUALIFIER
# define JSONCPP_VERSION_HEXA ((JSONCPP_VERSION_MAJOR << 24) | (JSONCPP_VERSION_MINOR << 16) | (JSONCPP_VERSION_PATCH << 8))

#endif // JSON_VERSION_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/version.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/config.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSON_CONFIG_H_INCLUDED
#define JSON_CONFIG_H_INCLUDED

/// If defined, indicates that json library is embedded in CppTL library.
//# define JSON_IN_CPPTL 1

/// If defined, indicates that json may leverage CppTL library
//#  define JSON_USE_CPPTL 1
/// If defined, indicates that cpptl vector based map should be used instead of
/// std::map
/// as Value container.
//#  define JSON_USE_CPPTL_SMALLMAP 1
/// If defined, indicates that Json specific container should be used
/// (hash table & simple deque container with customizable allocator).
/// THIS FEATURE IS STILL EXPERIMENTAL! There is know bugs: See #3177332
//#  define JSON_VALUE_USE_INTERNAL_MAP 1
/// Force usage of standard new/malloc based allocator instead of memory pool
/// based allocator.
/// The memory pools allocator used optimization (initializing Value and
/// ValueInternalLink
/// as if it was a POD) that may cause some validation tool to report errors.
/// Only has effects if JSON_VALUE_USE_INTERNAL_MAP is defined.
//#  define JSON_USE_SIMPLE_INTERNAL_ALLOCATOR 1

// If non-zero, the library uses exceptions to report bad input instead of C
// assertion macros. The default is to use exceptions.
#ifndef JSON_USE_EXCEPTION
#define JSON_USE_EXCEPTION 1
#endif

/// If defined, indicates that the source file is amalgated
/// to prevent private header inclusion.
/// Remarks: it is automatically defined in the generated amalgated header.
// #define JSON_IS_AMALGAMATION

#ifdef JSON_IN_CPPTL
#include <cpptl/config.h>
#ifndef JSON_USE_CPPTL
#define JSON_USE_CPPTL 1
#endif
#endif

#ifdef JSON_IN_CPPTL
#define JSON_API CPPTL_API
#elif defined(JSON_DLL_BUILD)
#if defined(_MSC_VER)
#define JSON_API __declspec(dllexport)
#define JSONCPP_DISABLE_DLL_INTERFACE_WARNING
#endif // if defined(_MSC_VER)
#elif defined(JSON_DLL)
#if defined(_MSC_VER)
#define JSON_API __declspec(dllimport)
#define JSONCPP_DISABLE_DLL_INTERFACE_WARNING
#endif // if defined(_MSC_VER)
#endif // ifdef JSON_IN_CPPTL
#if !defined(JSON_API)
#define JSON_API
#endif

// If JSON_NO_INT64 is defined, then Json only support C++ "int" type for
// integer
// Storages, and 64 bits integer support is disabled.
// #define JSON_NO_INT64 1

#if defined(_MSC_VER) && _MSC_VER <= 1200 // MSVC 6
// Microsoft Visual Studio 6 only support conversion from __int64 to double
// (no conversion from unsigned __int64).
#define JSON_USE_INT64_DOUBLE_CONVERSION 1
// Disable warning 4786 for VS6 caused by STL (identifier was truncated to '255'
// characters in the debug information)
// All projects I've ever seen with VS6 were using this globally (not bothering
// with pragma push/pop).
#pragma warning(disable : 4786)
#endif // if defined(_MSC_VER)  &&  _MSC_VER < 1200 // MSVC 6

#if defined(_MSC_VER) && _MSC_VER >= 1500 // MSVC 2008
/// Indicates that the following function is deprecated.
#define JSONCPP_DEPRECATED(message) __declspec(deprecated(message))
#endif

#if !defined(JSONCPP_DEPRECATED)
#define JSONCPP_DEPRECATED(message)
#endif // if !defined(JSONCPP_DEPRECATED)

namespace Json {
typedef int Int;
typedef unsigned int UInt;
#if defined(JSON_NO_INT64)
typedef int LargestInt;
typedef unsigned int LargestUInt;
#undef JSON_HAS_INT64
#else                 // if defined(JSON_NO_INT64)
// For Microsoft Visual use specific types as long long is not supported
#if defined(_MSC_VER) // Microsoft Visual Studio
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#else                 // if defined(_MSC_VER) // Other platforms, use long long
typedef long long int Int64;
typedef unsigned long long int UInt64;
#endif // if defined(_MSC_VER)
typedef Int64 LargestInt;
typedef UInt64 LargestUInt;
#define JSON_HAS_INT64
#endif // if defined(JSON_NO_INT64)
} // end namespace Json

#endif // JSON_CONFIG_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/config.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/forwards.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSON_FORWARDS_H_INCLUDED
#define JSON_FORWARDS_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "config.h"
#endif // if !defined(JSON_IS_AMALGAMATION)

namespace Json {

// writer.h
class FastWriter;
class StyledWriter;

// reader.h
class Reader;

// features.h
class Features;

// value.h
typedef unsigned int ArrayIndex;
class StaticString;
class Path;
class PathArgument;
class Value;
class ValueIteratorBase;
class ValueIterator;
class ValueConstIterator;
#ifdef JSON_VALUE_USE_INTERNAL_MAP
class ValueMapAllocator;
class ValueInternalLink;
class ValueInternalArray;
class ValueInternalMap;
#endif // #ifdef JSON_VALUE_USE_INTERNAL_MAP

} // namespace Json

#endif // JSON_FORWARDS_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/forwards.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/features.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef CPPTL_JSON_FEATURES_H_INCLUDED
#define CPPTL_JSON_FEATURES_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "forwards.h"
#endif // if !defined(JSON_IS_AMALGAMATION)

namespace Json {

/** \brief Configuration passed to reader and writer.
 * This configuration object can be used to force the Reader or Writer
 * to behave in a standard conforming way.
 */
class JSON_API Features {
public:
  /** \brief A configuration that allows all features and assumes all strings
   * are UTF-8.
   * - C & C++ comments are allowed
   * - Root object can be any JSON value
   * - Assumes Value strings are encoded in UTF-8
   */
  static Features all();

  /** \brief A configuration that is strictly compatible with the JSON
   * specification.
   * - Comments are forbidden.
   * - Root object must be either an array or an object value.
   * - Assumes Value strings are encoded in UTF-8
   */
  static Features strictMode();

  /** \brief Initialize the configuration like JsonConfig::allFeatures;
   */
  Features();

  /// \c true if comments are allowed. Default: \c true.
  bool allowComments_;

  /// \c true if root must be either an array or an object value. Default: \c
  /// false.
  bool strictRoot_;

  /// \c true if dropped null placeholders are allowed. Default: \c false.
  bool allowDroppedNullPlaceholders_;

  /// \c true if numeric object key are allowed. Default: \c false.
  bool allowNumericKeys_;
};

} // namespace Json

#endif // CPPTL_JSON_FEATURES_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/features.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/value.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef CPPTL_JSON_H_INCLUDED
#define CPPTL_JSON_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "forwards.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <string>
#include <vector>

#ifndef JSON_USE_CPPTL_SMALLMAP
#include <map>
#else
#include <cpptl/smallmap.h>
#endif
#ifdef JSON_USE_CPPTL
#include <cpptl/forwards.h>
#endif

// Disable warning C4251: <data member>: <type> needs to have dll-interface to
// be used by...
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

/** \brief JSON (JavaScript Object Notation).
 */
namespace Json {

/** \brief Type of the value held by a Value object.
 */
enum ValueType {
  nullValue = 0, ///< 'null' value
  intValue,      ///< signed integer value
  uintValue,     ///< unsigned integer value
  realValue,     ///< double value
  stringValue,   ///< UTF-8 string value
  booleanValue,  ///< bool value
  arrayValue,    ///< array value (ordered list)
  objectValue    ///< object value (collection of name/value pairs).
};

enum CommentPlacement {
  commentBefore = 0,      ///< a comment placed on the line before a value
  commentAfterOnSameLine, ///< a comment just after a value on the same line
  commentAfter, ///< a comment on the line after a value (only make sense for
  /// root value)
  numberOfCommentPlacement
};

//# ifdef JSON_USE_CPPTL
//   typedef CppTL::AnyEnumerator<const char *> EnumMemberNames;
//   typedef CppTL::AnyEnumerator<const Value &> EnumValues;
//# endif

/** \brief Lightweight wrapper to tag static string.
 *
 * Value constructor and objectValue member assignement takes advantage of the
 * StaticString and avoid the cost of string duplication when storing the
 * string or the member name.
 *
 * Example of usage:
 * \code
 * Json::Value aValue( StaticString("some text") );
 * Json::Value object;
 * static const StaticString code("code");
 * object[code] = 1234;
 * \endcode
 */
class JSON_API StaticString {
public:
  explicit StaticString(const char* czstring) : str_(czstring) {}

  operator const char*() const { return str_; }

  const char* c_str() const { return str_; }

private:
  const char* str_;
};

/** \brief Represents a <a HREF="http://www.json.org">JSON</a> value.
 *
 * This class is a discriminated union wrapper that can represents a:
 * - signed integer [range: Value::minInt - Value::maxInt]
 * - unsigned integer (range: 0 - Value::maxUInt)
 * - double
 * - UTF-8 string
 * - boolean
 * - 'null'
 * - an ordered list of Value
 * - collection of name/value pairs (javascript object)
 *
 * The type of the held value is represented by a #ValueType and
 * can be obtained using type().
 *
 * values of an #objectValue or #arrayValue can be accessed using operator[]()
 *methods.
 * Non const methods will automatically create the a #nullValue element
 * if it does not exist.
 * The sequence of an #arrayValue will be automatically resize and initialized
 * with #nullValue. resize() can be used to enlarge or truncate an #arrayValue.
 *
 * The get() methods can be used to obtanis default value in the case the
 *required element
 * does not exist.
 *
 * It is possible to iterate over the list of a #objectValue values using
 * the getMemberNames() method.
 */
class JSON_API Value {
  friend class ValueIteratorBase;
#ifdef JSON_VALUE_USE_INTERNAL_MAP
  friend class ValueInternalLink;
  friend class ValueInternalMap;
#endif
public:
  typedef std::vector<std::string> Members;
  typedef ValueIterator iterator;
  typedef ValueConstIterator const_iterator;
  typedef Json::UInt UInt;
  typedef Json::Int Int;
#if defined(JSON_HAS_INT64)
  typedef Json::UInt64 UInt64;
  typedef Json::Int64 Int64;
#endif // defined(JSON_HAS_INT64)
  typedef Json::LargestInt LargestInt;
  typedef Json::LargestUInt LargestUInt;
  typedef Json::ArrayIndex ArrayIndex;

  static const Value& null;
  /// Minimum signed integer value that can be stored in a Json::Value.
  static const LargestInt minLargestInt;
  /// Maximum signed integer value that can be stored in a Json::Value.
  static const LargestInt maxLargestInt;
  /// Maximum unsigned integer value that can be stored in a Json::Value.
  static const LargestUInt maxLargestUInt;

  /// Minimum signed int value that can be stored in a Json::Value.
  static const Int minInt;
  /// Maximum signed int value that can be stored in a Json::Value.
  static const Int maxInt;
  /// Maximum unsigned int value that can be stored in a Json::Value.
  static const UInt maxUInt;

#if defined(JSON_HAS_INT64)
  /// Minimum signed 64 bits int value that can be stored in a Json::Value.
  static const Int64 minInt64;
  /// Maximum signed 64 bits int value that can be stored in a Json::Value.
  static const Int64 maxInt64;
  /// Maximum unsigned 64 bits int value that can be stored in a Json::Value.
  static const UInt64 maxUInt64;
#endif // defined(JSON_HAS_INT64)

private:
#ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  class CZString {
  public:
	enum DuplicationPolicy {
	  noDuplication = 0,
	  duplicate,
	  duplicateOnCopy
	};
	CZString(ArrayIndex index);
	CZString(const char* cstr, DuplicationPolicy allocate);
	CZString(const CZString& other);
	~CZString();
	CZString& operator=(CZString other);
	bool operator<(const CZString& other) const;
	bool operator==(const CZString& other) const;
	ArrayIndex index() const;
	const char* c_str() const;
	bool isStaticString() const;

  private:
	void swap(CZString& other);
	const char* cstr_;
	ArrayIndex index_;
  };

public:
#ifndef JSON_USE_CPPTL_SMALLMAP
  typedef std::map<CZString, Value> ObjectValues;
#else
  typedef CppTL::SmallMap<CZString, Value> ObjectValues;
#endif // ifndef JSON_USE_CPPTL_SMALLMAP
#endif // ifndef JSON_VALUE_USE_INTERNAL_MAP
#endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

public:
  /** \brief Create a default Value of the given type.

	This is a very useful constructor.
	To create an empty array, pass arrayValue.
	To create an empty object, pass objectValue.
	Another Value can then be set to this one by assignment.
This is useful since clear() and resize() will not alter types.

	Examples:
\code
Json::Value null_value; // null
Json::Value arr_value(Json::arrayValue); // []
Json::Value obj_value(Json::objectValue); // {}
\endcode
  */
  Value(ValueType type = nullValue);
  Value(Int value);
  Value(UInt value);
#if defined(JSON_HAS_INT64)
  Value(Int64 value);
  Value(UInt64 value);
#endif // if defined(JSON_HAS_INT64)
  Value(double value);
  Value(const char* value);
  Value(const char* beginValue, const char* endValue);
  /** \brief Constructs a value from a static string.

   * Like other value string constructor but do not duplicate the string for
   * internal storage. The given string must remain alive after the call to this
   * constructor.
   * Example of usage:
   * \code
   * Json::Value aValue( StaticString("some text") );
   * \endcode
   */
  Value(const StaticString& value);
  Value(const std::string& value);
#ifdef JSON_USE_CPPTL
  Value(const CppTL::ConstString& value);
#endif
  Value(bool value);
  Value(const Value& other);
  ~Value();

  Value& operator=(Value other);
  /// Swap values.
  /// \note Currently, comments are intentionally not swapped, for
  /// both logic and efficiency.
  void swap(Value& other);

  ValueType type() const;

  bool operator<(const Value& other) const;
  bool operator<=(const Value& other) const;
  bool operator>=(const Value& other) const;
  bool operator>(const Value& other) const;

  bool operator==(const Value& other) const;
  bool operator!=(const Value& other) const;

  int compare(const Value& other) const;

  const char* asCString() const;
  std::string asString() const;
#ifdef JSON_USE_CPPTL
  CppTL::ConstString asConstString() const;
#endif
  Int asInt() const;
  UInt asUInt() const;
#if defined(JSON_HAS_INT64)
  Int64 asInt64() const;
  UInt64 asUInt64() const;
#endif // if defined(JSON_HAS_INT64)
  LargestInt asLargestInt() const;
  LargestUInt asLargestUInt() const;
  float asFloat() const;
  double asDouble() const;
  bool asBool() const;

  bool isNull() const;
  bool isBool() const;
  bool isInt() const;
  bool isInt64() const;
  bool isUInt() const;
  bool isUInt64() const;
  bool isIntegral() const;
  bool isDouble() const;
  bool isNumeric() const;
  bool isString() const;
  bool isArray() const;
  bool isObject() const;

  bool isConvertibleTo(ValueType other) const;

  /// Number of values in array or object
  ArrayIndex size() const;

  /// \brief Return true if empty array, empty object, or null;
  /// otherwise, false.
  bool empty() const;

  /// Return isNull()
  bool operator!() const;

  /// Remove all object members and array elements.
  /// \pre type() is arrayValue, objectValue, or nullValue
  /// \post type() is unchanged
  void clear();

  /// Resize the array to size elements.
  /// New elements are initialized to null.
  /// May only be called on nullValue or arrayValue.
  /// \pre type() is arrayValue or nullValue
  /// \post type() is arrayValue
  void resize(ArrayIndex size);

  /// Access an array element (zero based index ).
  /// If the array contains less than index element, then null value are
  /// inserted
  /// in the array so that its size is index+1.
  /// (You may need to say 'value[0u]' to get your compiler to distinguish
  ///  this from the operator[] which takes a string.)
  Value& operator[](ArrayIndex index);

  /// Access an array element (zero based index ).
  /// If the array contains less than index element, then null value are
  /// inserted
  /// in the array so that its size is index+1.
  /// (You may need to say 'value[0u]' to get your compiler to distinguish
  ///  this from the operator[] which takes a string.)
  Value& operator[](int index);

  /// Access an array element (zero based index )
  /// (You may need to say 'value[0u]' to get your compiler to distinguish
  ///  this from the operator[] which takes a string.)
  const Value& operator[](ArrayIndex index) const;

  /// Access an array element (zero based index )
  /// (You may need to say 'value[0u]' to get your compiler to distinguish
  ///  this from the operator[] which takes a string.)
  const Value& operator[](int index) const;

  /// If the array contains at least index+1 elements, returns the element
  /// value,
  /// otherwise returns defaultValue.
  Value get(ArrayIndex index, const Value& defaultValue) const;
  /// Return true if index < size().
  bool isValidIndex(ArrayIndex index) const;
  /// \brief Append value to array at the end.
  ///
  /// Equivalent to jsonvalue[jsonvalue.size()] = value;
  Value& append(const Value& value);

  /// Access an object value by name, create a null member if it does not exist.
  Value& operator[](const char* key);
  /// Access an object value by name, returns null if there is no member with
  /// that name.
  const Value& operator[](const char* key) const;
  /// Access an object value by name, create a null member if it does not exist.
  Value& operator[](const std::string& key);
  /// Access an object value by name, returns null if there is no member with
  /// that name.
  const Value& operator[](const std::string& key) const;
  /** \brief Access an object value by name, create a null member if it does not
   exist.

   * If the object as no entry for that name, then the member name used to store
   * the new entry is not duplicated.
   * Example of use:
   * \code
   * Json::Value object;
   * static const StaticString code("code");
   * object[code] = 1234;
   * \endcode
   */
  Value& operator[](const StaticString& key);
#ifdef JSON_USE_CPPTL
  /// Access an object value by name, create a null member if it does not exist.
  Value& operator[](const CppTL::ConstString& key);
  /// Access an object value by name, returns null if there is no member with
  /// that name.
  const Value& operator[](const CppTL::ConstString& key) const;
#endif
  /// Return the member named key if it exist, defaultValue otherwise.
  Value get(const char* key, const Value& defaultValue) const;
  /// Return the member named key if it exist, defaultValue otherwise.
  Value get(const std::string& key, const Value& defaultValue) const;
#ifdef JSON_USE_CPPTL
  /// Return the member named key if it exist, defaultValue otherwise.
  Value get(const CppTL::ConstString& key, const Value& defaultValue) const;
#endif
  /// \brief Remove and return the named member.
  ///
  /// Do nothing if it did not exist.
  /// \return the removed Value, or null.
  /// \pre type() is objectValue or nullValue
  /// \post type() is unchanged
  Value removeMember(const char* key);
  /// Same as removeMember(const char*)
  Value removeMember(const std::string& key);

  /// Return true if the object has a member named key.
  bool isMember(const char* key) const;
  /// Return true if the object has a member named key.
  bool isMember(const std::string& key) const;
#ifdef JSON_USE_CPPTL
  /// Return true if the object has a member named key.
  bool isMember(const CppTL::ConstString& key) const;
#endif

  /// \brief Return a list of the member names.
  ///
  /// If null, return an empty list.
  /// \pre type() is objectValue or nullValue
  /// \post if type() was nullValue, it remains nullValue
  Members getMemberNames() const;

  //# ifdef JSON_USE_CPPTL
  //      EnumMemberNames enumMemberNames() const;
  //      EnumValues enumValues() const;
  //# endif

  /// Comments must be //... or /* ... */
  void setComment(const char* comment, CommentPlacement placement);
  /// Comments must be //... or /* ... */
  void setComment(const std::string& comment, CommentPlacement placement);
  bool hasComment(CommentPlacement placement) const;
  /// Include delimiters and embedded newlines.
  std::string getComment(CommentPlacement placement) const;

  std::string toStyledString() const;

  const_iterator begin() const;
  const_iterator end() const;

  iterator begin();
  iterator end();

  // Accessors for the [start, limit) range of bytes within the JSON text from
  // which this value was parsed, if any.
  void setOffsetStart(size_t start);
  void setOffsetLimit(size_t limit);
  size_t getOffsetStart() const;
  size_t getOffsetLimit() const;

private:
  Value& resolveReference(const char* key, bool isStatic);

#ifdef JSON_VALUE_USE_INTERNAL_MAP
  inline bool isItemAvailable() const { return itemIsUsed_ == 0; }

  inline void setItemUsed(bool isUsed = true) { itemIsUsed_ = isUsed ? 1 : 0; }

  inline bool isMemberNameStatic() const { return memberNameIsStatic_ == 0; }

  inline void setMemberNameIsStatic(bool isStatic) {
	memberNameIsStatic_ = isStatic ? 1 : 0;
  }
#endif // # ifdef JSON_VALUE_USE_INTERNAL_MAP

private:
  struct CommentInfo {
	CommentInfo();
	~CommentInfo();

	void setComment(const char* text);

	char* comment_;
  };

  // struct MemberNamesTransform
  //{
  //   typedef const char *result_type;
  //   const char *operator()( const CZString &name ) const
  //   {
  //      return name.c_str();
  //   }
  //};

  union ValueHolder {
	LargestInt int_;
	LargestUInt uint_;
	double real_;
	bool bool_;
	char* string_;
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	ValueInternalArray* array_;
	ValueInternalMap* map_;
#else
	ObjectValues* map_;
#endif
  } value_;
  ValueType type_ : 8;
  int allocated_ : 1; // Notes: if declared as bool, bitfield is useless.
#ifdef JSON_VALUE_USE_INTERNAL_MAP
  unsigned int itemIsUsed_ : 1; // used by the ValueInternalMap container.
  int memberNameIsStatic_ : 1;  // used by the ValueInternalMap container.
#endif
  CommentInfo* comments_;

  // [start, limit) byte offsets in the source JSON text from which this Value
  // was extracted.
  size_t start_;
  size_t limit_;
};

/** \brief Experimental and untested: represents an element of the "path" to
 * access a node.
 */
class JSON_API PathArgument {
public:
  friend class Path;

  PathArgument();
  PathArgument(ArrayIndex index);
  PathArgument(const char* key);
  PathArgument(const std::string& key);

private:
  enum Kind {
	kindNone = 0,
	kindIndex,
	kindKey
  };
  std::string key_;
  ArrayIndex index_;
  Kind kind_;
};

/** \brief Experimental and untested: represents a "path" to access a node.
 *
 * Syntax:
 * - "." => root node
 * - ".[n]" => elements at index 'n' of root node (an array value)
 * - ".name" => member named 'name' of root node (an object value)
 * - ".name1.name2.name3"
 * - ".[0][1][2].name1[3]"
 * - ".%" => member name is provided as parameter
 * - ".[%]" => index is provied as parameter
 */
class JSON_API Path {
public:
  Path(const std::string& path,
	   const PathArgument& a1 = PathArgument(),
	   const PathArgument& a2 = PathArgument(),
	   const PathArgument& a3 = PathArgument(),
	   const PathArgument& a4 = PathArgument(),
	   const PathArgument& a5 = PathArgument());

  const Value& resolve(const Value& root) const;
  Value resolve(const Value& root, const Value& defaultValue) const;
  /// Creates the "path" to access the specified node and returns a reference on
  /// the node.
  Value& make(Value& root) const;

private:
  typedef std::vector<const PathArgument*> InArgs;
  typedef std::vector<PathArgument> Args;

  void makePath(const std::string& path, const InArgs& in);
  void addPathInArg(const std::string& path,
					const InArgs& in,
					InArgs::const_iterator& itInArg,
					PathArgument::Kind kind);
  void invalidPath(const std::string& path, int location);

  Args args_;
};

#ifdef JSON_VALUE_USE_INTERNAL_MAP
/** \brief Allocator to customize Value internal map.
 * Below is an example of a simple implementation (default implementation
 actually
 * use memory pool for speed).
 * \code
   class DefaultValueMapAllocator : public ValueMapAllocator
   {
   public: // overridden from ValueMapAllocator
	  virtual ValueInternalMap *newMap()
	  {
		 return new ValueInternalMap();
	  }

	  virtual ValueInternalMap *newMapCopy( const ValueInternalMap &other )
	  {
		 return new ValueInternalMap( other );
	  }

	  virtual void destructMap( ValueInternalMap *map )
	  {
		 delete map;
	  }

	  virtual ValueInternalLink *allocateMapBuckets( unsigned int size )
	  {
		 return new ValueInternalLink[size];
	  }

	  virtual void releaseMapBuckets( ValueInternalLink *links )
	  {
		 delete [] links;
	  }

	  virtual ValueInternalLink *allocateMapLink()
	  {
		 return new ValueInternalLink();
	  }

	  virtual void releaseMapLink( ValueInternalLink *link )
	  {
		 delete link;
	  }
   };
 * \endcode
 */
class JSON_API ValueMapAllocator {
public:
  virtual ~ValueMapAllocator();
  virtual ValueInternalMap* newMap() = 0;
  virtual ValueInternalMap* newMapCopy(const ValueInternalMap& other) = 0;
  virtual void destructMap(ValueInternalMap* map) = 0;
  virtual ValueInternalLink* allocateMapBuckets(unsigned int size) = 0;
  virtual void releaseMapBuckets(ValueInternalLink* links) = 0;
  virtual ValueInternalLink* allocateMapLink() = 0;
  virtual void releaseMapLink(ValueInternalLink* link) = 0;
};

/** \brief ValueInternalMap hash-map bucket chain link (for internal use only).
 * \internal previous_ & next_ allows for bidirectional traversal.
 */
class JSON_API ValueInternalLink {
public:
  enum {
	itemPerLink = 6
  }; // sizeof(ValueInternalLink) = 128 on 32 bits architecture.
  enum InternalFlags {
	flagAvailable = 0,
	flagUsed = 1
  };

  ValueInternalLink();

  ~ValueInternalLink();

  Value items_[itemPerLink];
  char* keys_[itemPerLink];
  ValueInternalLink* previous_;
  ValueInternalLink* next_;
};

/** \brief A linked page based hash-table implementation used internally by
 *Value.
 * \internal ValueInternalMap is a tradional bucket based hash-table, with a
 *linked
 * list in each bucket to handle collision. There is an addional twist in that
 * each node of the collision linked list is a page containing a fixed amount of
 * value. This provides a better compromise between memory usage and speed.
 *
 * Each bucket is made up of a chained list of ValueInternalLink. The last
 * link of a given bucket can be found in the 'previous_' field of the following
 *bucket.
 * The last link of the last bucket is stored in tailLink_ as it has no
 *following bucket.
 * Only the last link of a bucket may contains 'available' item. The last link
 *always
 * contains at least one element unless is it the bucket one very first link.
 */
class JSON_API ValueInternalMap {
  friend class ValueIteratorBase;
  friend class Value;

public:
  typedef unsigned int HashKey;
  typedef unsigned int BucketIndex;

#ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
  struct IteratorState {
	IteratorState() : map_(0), link_(0), itemIndex_(0), bucketIndex_(0) {}
	ValueInternalMap* map_;
	ValueInternalLink* link_;
	BucketIndex itemIndex_;
	BucketIndex bucketIndex_;
  };
#endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

  ValueInternalMap();
  ValueInternalMap(const ValueInternalMap& other);
  ValueInternalMap& operator=(ValueInternalMap other);
  ~ValueInternalMap();

  void swap(ValueInternalMap& other);

  BucketIndex size() const;

  void clear();

  bool reserveDelta(BucketIndex growth);

  bool reserve(BucketIndex newItemCount);

  const Value* find(const char* key) const;

  Value* find(const char* key);

  Value& resolveReference(const char* key, bool isStatic);

  void remove(const char* key);

  void doActualRemove(ValueInternalLink* link,
					  BucketIndex index,
					  BucketIndex bucketIndex);

  ValueInternalLink*& getLastLinkInBucket(BucketIndex bucketIndex);

  Value& setNewItem(const char* key,
					bool isStatic,
					ValueInternalLink* link,
					BucketIndex index);

  Value& unsafeAdd(const char* key, bool isStatic, HashKey hashedKey);

  HashKey hash(const char* key) const;

  int compare(const ValueInternalMap& other) const;

private:
  void makeBeginIterator(IteratorState& it) const;
  void makeEndIterator(IteratorState& it) const;
  static bool equals(const IteratorState& x, const IteratorState& other);
  static void increment(IteratorState& iterator);
  static void incrementBucket(IteratorState& iterator);
  static void decrement(IteratorState& iterator);
  static const char* key(const IteratorState& iterator);
  static const char* key(const IteratorState& iterator, bool& isStatic);
  static Value& value(const IteratorState& iterator);
  static int distance(const IteratorState& x, const IteratorState& y);

private:
  ValueInternalLink* buckets_;
  ValueInternalLink* tailLink_;
  BucketIndex bucketsSize_;
  BucketIndex itemCount_;
};

/** \brief A simplified deque implementation used internally by Value.
* \internal
* It is based on a list of fixed "page", each page contains a fixed number of
*items.
* Instead of using a linked-list, a array of pointer is used for fast item
*look-up.
* Look-up for an element is as follow:
* - compute page index: pageIndex = itemIndex / itemsPerPage
* - look-up item in page: pages_[pageIndex][itemIndex % itemsPerPage]
*
* Insertion is amortized constant time (only the array containing the index of
*pointers
* need to be reallocated when items are appended).
*/
class JSON_API ValueInternalArray {
  friend class Value;
  friend class ValueIteratorBase;

public:
  enum {
	itemsPerPage = 8
  }; // should be a power of 2 for fast divide and modulo.
  typedef Value::ArrayIndex ArrayIndex;
  typedef unsigned int PageIndex;

#ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
  struct IteratorState // Must be a POD
	  {
	IteratorState() : array_(0), currentPageIndex_(0), currentItemIndex_(0) {}
	ValueInternalArray* array_;
	Value** currentPageIndex_;
	unsigned int currentItemIndex_;
  };
#endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

  ValueInternalArray();
  ValueInternalArray(const ValueInternalArray& other);
  ValueInternalArray& operator=(ValueInternalArray other);
  ~ValueInternalArray();
  void swap(ValueInternalArray& other);

  void clear();
  void resize(ArrayIndex newSize);

  Value& resolveReference(ArrayIndex index);

  Value* find(ArrayIndex index) const;

  ArrayIndex size() const;

  int compare(const ValueInternalArray& other) const;

private:
  static bool equals(const IteratorState& x, const IteratorState& other);
  static void increment(IteratorState& iterator);
  static void decrement(IteratorState& iterator);
  static Value& dereference(const IteratorState& iterator);
  static Value& unsafeDereference(const IteratorState& iterator);
  static int distance(const IteratorState& x, const IteratorState& y);
  static ArrayIndex indexOf(const IteratorState& iterator);
  void makeBeginIterator(IteratorState& it) const;
  void makeEndIterator(IteratorState& it) const;
  void makeIterator(IteratorState& it, ArrayIndex index) const;

  void makeIndexValid(ArrayIndex index);

  Value** pages_;
  ArrayIndex size_;
  PageIndex pageCount_;
};

/** \brief Experimental: do not use. Allocator to customize Value internal
array.
 * Below is an example of a simple implementation (actual implementation use
 * memory pool).
   \code
class DefaultValueArrayAllocator : public ValueArrayAllocator
{
public: // overridden from ValueArrayAllocator
virtual ~DefaultValueArrayAllocator()
{
}

virtual ValueInternalArray *newArray()
{
   return new ValueInternalArray();
}

virtual ValueInternalArray *newArrayCopy( const ValueInternalArray &other )
{
   return new ValueInternalArray( other );
}

virtual void destruct( ValueInternalArray *array )
{
   delete array;
}

virtual void reallocateArrayPageIndex( Value **&indexes,
									   ValueInternalArray::PageIndex
&indexCount,
									   ValueInternalArray::PageIndex
minNewIndexCount )
{
   ValueInternalArray::PageIndex newIndexCount = (indexCount*3)/2 + 1;
   if ( minNewIndexCount > newIndexCount )
	  newIndexCount = minNewIndexCount;
   void *newIndexes = realloc( indexes, sizeof(Value*) * newIndexCount );
   if ( !newIndexes )
	  throw std::bad_alloc();
   indexCount = newIndexCount;
   indexes = static_cast<Value **>( newIndexes );
}
virtual void releaseArrayPageIndex( Value **indexes,
									ValueInternalArray::PageIndex indexCount )
{
   if ( indexes )
	  free( indexes );
}

virtual Value *allocateArrayPage()
{
   return static_cast<Value *>( malloc( sizeof(Value) *
ValueInternalArray::itemsPerPage ) );
}

virtual void releaseArrayPage( Value *value )
{
   if ( value )
	  free( value );
}
};
   \endcode
 */
class JSON_API ValueArrayAllocator {
public:
  virtual ~ValueArrayAllocator();
  virtual ValueInternalArray* newArray() = 0;
  virtual ValueInternalArray* newArrayCopy(const ValueInternalArray& other) = 0;
  virtual void destructArray(ValueInternalArray* array) = 0;
  /** \brief Reallocate array page index.
   * Reallocates an array of pointer on each page.
   * \param indexes [input] pointer on the current index. May be \c NULL.
   *                [output] pointer on the new index of at least
   *                         \a minNewIndexCount pages.
   * \param indexCount [input] current number of pages in the index.
   *                   [output] number of page the reallocated index can handle.
   *                            \b MUST be >= \a minNewIndexCount.
   * \param minNewIndexCount Minimum number of page the new index must be able
   * to
   *                         handle.
   */
  virtual void
  reallocateArrayPageIndex(Value**& indexes,
						   ValueInternalArray::PageIndex& indexCount,
						   ValueInternalArray::PageIndex minNewIndexCount) = 0;
  virtual void
  releaseArrayPageIndex(Value** indexes,
						ValueInternalArray::PageIndex indexCount) = 0;
  virtual Value* allocateArrayPage() = 0;
  virtual void releaseArrayPage(Value* value) = 0;
};
#endif // #ifdef JSON_VALUE_USE_INTERNAL_MAP

/** \brief base class for Value iterators.
 *
 */
class JSON_API ValueIteratorBase {
public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef unsigned int size_t;
  typedef int difference_type;
  typedef ValueIteratorBase SelfType;

  ValueIteratorBase();
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  explicit ValueIteratorBase(const Value::ObjectValues::iterator& current);
#else
  ValueIteratorBase(const ValueInternalArray::IteratorState& state);
  ValueIteratorBase(const ValueInternalMap::IteratorState& state);
#endif

  bool operator==(const SelfType& other) const { return isEqual(other); }

  bool operator!=(const SelfType& other) const { return !isEqual(other); }

  difference_type operator-(const SelfType& other) const {
	return computeDistance(other);
  }

  /// Return either the index or the member name of the referenced value as a
  /// Value.
  Value key() const;

  /// Return the index of the referenced Value. -1 if it is not an arrayValue.
  UInt index() const;

  /// Return the member name of the referenced Value. "" if it is not an
  /// objectValue.
  const char* memberName() const;

protected:
  Value& deref() const;

  void increment();

  void decrement();

  difference_type computeDistance(const SelfType& other) const;

  bool isEqual(const SelfType& other) const;

  void copy(const SelfType& other);

private:
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  Value::ObjectValues::iterator current_;
  // Indicates that iterator is for a null value.
  bool isNull_;
#else
  union {
	ValueInternalArray::IteratorState array_;
	ValueInternalMap::IteratorState map_;
  } iterator_;
  bool isArray_;
#endif
};

/** \brief const iterator for object and array value.
 *
 */
class JSON_API ValueConstIterator : public ValueIteratorBase {
  friend class Value;

public:
  typedef const Value value_type;
  typedef unsigned int size_t;
  typedef int difference_type;
  typedef const Value& reference;
  typedef const Value* pointer;
  typedef ValueConstIterator SelfType;

  ValueConstIterator();

private:
/*! \internal Use by Value to create an iterator.
 */
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  explicit ValueConstIterator(const Value::ObjectValues::iterator& current);
#else
  ValueConstIterator(const ValueInternalArray::IteratorState& state);
  ValueConstIterator(const ValueInternalMap::IteratorState& state);
#endif
public:
  SelfType& operator=(const ValueIteratorBase& other);

  SelfType operator++(int) {
	SelfType temp(*this);
	++*this;
	return temp;
  }

  SelfType operator--(int) {
	SelfType temp(*this);
	--*this;
	return temp;
  }

  SelfType& operator--() {
	decrement();
	return *this;
  }

  SelfType& operator++() {
	increment();
	return *this;
  }

  reference operator*() const { return deref(); }

  pointer operator->() const { return &deref(); }
};

/** \brief Iterator for object and array value.
 */
class JSON_API ValueIterator : public ValueIteratorBase {
  friend class Value;

public:
  typedef Value value_type;
  typedef unsigned int size_t;
  typedef int difference_type;
  typedef Value& reference;
  typedef Value* pointer;
  typedef ValueIterator SelfType;

  ValueIterator();
  ValueIterator(const ValueConstIterator& other);
  ValueIterator(const ValueIterator& other);

private:
/*! \internal Use by Value to create an iterator.
 */
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  explicit ValueIterator(const Value::ObjectValues::iterator& current);
#else
  ValueIterator(const ValueInternalArray::IteratorState& state);
  ValueIterator(const ValueInternalMap::IteratorState& state);
#endif
public:
  SelfType& operator=(const SelfType& other);

  SelfType operator++(int) {
	SelfType temp(*this);
	++*this;
	return temp;
  }

  SelfType operator--(int) {
	SelfType temp(*this);
	--*this;
	return temp;
  }

  SelfType& operator--() {
	decrement();
	return *this;
  }

  SelfType& operator++() {
	increment();
	return *this;
  }

  reference operator*() const { return deref(); }

  pointer operator->() const { return &deref(); }
};

} // namespace Json

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(pop)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

#endif // CPPTL_JSON_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/value.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/reader.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef CPPTL_JSON_READER_H_INCLUDED
#define CPPTL_JSON_READER_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "features.h"
#include "value.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <deque>
#include <iosfwd>
#include <stack>
#include <string>

// Disable warning C4251: <data member>: <type> needs to have dll-interface to
// be used by...
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

namespace Json {

/** \brief Unserialize a <a HREF="http://www.json.org">JSON</a> document into a
 *Value.
 *
 */
class JSON_API Reader {
public:
  typedef char Char;
  typedef const Char* Location;

  /** \brief An error tagged with where in the JSON text it was encountered.
   *
   * The offsets give the [start, limit) range of bytes within the text. Note
   * that this is bytes, not codepoints.
   *
   */
  struct StructuredError {
	size_t offset_start;
	size_t offset_limit;
	std::string message;
  };

  /** \brief Constructs a Reader allowing all features
   * for parsing.
   */
  Reader();

  /** \brief Constructs a Reader allowing the specified feature set
   * for parsing.
   */
  Reader(const Features& features);

  /** \brief Read a Value from a <a HREF="http://www.json.org">JSON</a>
   * document.
   * \param document UTF-8 encoded string containing the document to read.
   * \param root [out] Contains the root value of the document if it was
   *             successfully parsed.
   * \param collectComments \c true to collect comment and allow writing them
   * back during
   *                        serialization, \c false to discard comments.
   *                        This parameter is ignored if
   * Features::allowComments_
   *                        is \c false.
   * \return \c true if the document was successfully parsed, \c false if an
   * error occurred.
   */
  bool
  parse(const std::string& document, Value& root, bool collectComments = true);

  /** \brief Read a Value from a <a HREF="http://www.json.org">JSON</a>
   document.
   * \param beginDoc Pointer on the beginning of the UTF-8 encoded string of the
   document to read.
   * \param endDoc Pointer on the end of the UTF-8 encoded string of the
   document to read.
   \               Must be >= beginDoc.
   * \param root [out] Contains the root value of the document if it was
   *             successfully parsed.
   * \param collectComments \c true to collect comment and allow writing them
   back during
   *                        serialization, \c false to discard comments.
   *                        This parameter is ignored if
   Features::allowComments_
   *                        is \c false.
   * \return \c true if the document was successfully parsed, \c false if an
   error occurred.
   */
  bool parse(const char* beginDoc,
			 const char* endDoc,
			 Value& root,
			 bool collectComments = true);

  /// \brief Parse from input stream.
  /// \see Json::operator>>(std::istream&, Json::Value&).
  bool parse(std::istream& is, Value& root, bool collectComments = true);

  /** \brief Returns a user friendly string that list errors in the parsed
   * document.
   * \return Formatted error message with the list of errors with their location
   * in
   *         the parsed document. An empty string is returned if no error
   * occurred
   *         during parsing.
   * \deprecated Use getFormattedErrorMessages() instead (typo fix).
   */
  JSONCPP_DEPRECATED("Use getFormattedErrorMessages instead")
  std::string getFormatedErrorMessages() const;

  /** \brief Returns a user friendly string that list errors in the parsed
   * document.
   * \return Formatted error message with the list of errors with their location
   * in
   *         the parsed document. An empty string is returned if no error
   * occurred
   *         during parsing.
   */
  std::string getFormattedErrorMessages() const;

  /** \brief Returns a vector of structured erros encounted while parsing.
   * \return A (possibly empty) vector of StructuredError objects. Currently
   *         only one error can be returned, but the caller should tolerate
   * multiple
   *         errors.  This can occur if the parser recovers from a non-fatal
   *         parse error and then encounters additional errors.
   */
  std::vector<StructuredError> getStructuredErrors() const;

private:
  enum TokenType {
	tokenEndOfStream = 0,
	tokenObjectBegin,
	tokenObjectEnd,
	tokenArrayBegin,
	tokenArrayEnd,
	tokenString,
	tokenNumber,
	tokenTrue,
	tokenFalse,
	tokenNull,
	tokenArraySeparator,
	tokenMemberSeparator,
	tokenComment,
	tokenError
  };

  class Token {
  public:
	TokenType type_;
	Location start_;
	Location end_;
  };

  class ErrorInfo {
  public:
	Token token_;
	std::string message_;
	Location extra_;
  };

  typedef std::deque<ErrorInfo> Errors;

  bool expectToken(TokenType type, Token& token, const char* message);
  bool readToken(Token& token);
  void skipSpaces();
  bool match(Location pattern, int patternLength);
  bool readComment();
  bool readCStyleComment();
  bool readCppStyleComment();
  bool readString();
  void readNumber();
  bool readValue();
  bool readObject(Token& token);
  bool readArray(Token& token);
  bool decodeNumber(Token& token);
  bool decodeNumber(Token& token, Value& decoded);
  bool decodeString(Token& token);
  bool decodeString(Token& token, std::string& decoded);
  bool decodeDouble(Token& token);
  bool decodeDouble(Token& token, Value& decoded);
  bool decodeUnicodeCodePoint(Token& token,
							  Location& current,
							  Location end,
							  unsigned int& unicode);
  bool decodeUnicodeEscapeSequence(Token& token,
								   Location& current,
								   Location end,
								   unsigned int& unicode);
  bool addError(const std::string& message, Token& token, Location extra = 0);
  bool recoverFromError(TokenType skipUntilToken);
  bool addErrorAndRecover(const std::string& message,
						  Token& token,
						  TokenType skipUntilToken);
  void skipUntilSpace();
  Value& currentValue();
  Char getNextChar();
  void
  getLocationLineAndColumn(Location location, int& line, int& column) const;
  std::string getLocationLineAndColumn(Location location) const;
  void addComment(Location begin, Location end, CommentPlacement placement);
  void skipCommentTokens(Token& token);

  typedef std::stack<Value*> Nodes;
  Nodes nodes_;
  Errors errors_;
  std::string document_;
  Location begin_;
  Location end_;
  Location current_;
  Location lastValueEnd_;
  Value* lastValue_;
  std::string commentsBefore_;
  Features features_;
  bool collectComments_;
};

/** \brief Read from 'sin' into 'root'.

 Always keep comments from the input JSON.

 This can be used to read a file into a particular sub-object.
 For example:
 \code
 Json::Value root;
 cin >> root["dir"]["file"];
 cout << root;
 \endcode
 Result:
 \verbatim
 {
 "dir": {
	 "file": {
	 // The input stream JSON would be nested here.
	 }
 }
 }
 \endverbatim
 \throw std::exception on parse error.
 \see Json::operator<<()
*/
JSON_API std::istream& operator>>(std::istream&, Value&);

} // namespace Json

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(pop)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

#endif // CPPTL_JSON_READER_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/reader.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/writer.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSON_WRITER_H_INCLUDED
#define JSON_WRITER_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "value.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <vector>
#include <string>

// Disable warning C4251: <data member>: <type> needs to have dll-interface to
// be used by...
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

namespace Json {

class Value;

/** \brief Abstract class for writers.
 */
class JSON_API Writer {
public:
  virtual ~Writer();

  virtual std::string write(const Value& root) = 0;
};

/** \brief Outputs a Value in <a HREF="http://www.json.org">JSON</a> format
 *without formatting (not human friendly).
 *
 * The JSON document is written in a single line. It is not intended for 'human'
 *consumption,
 * but may be usefull to support feature such as RPC where bandwith is limited.
 * \sa Reader, Value
 */
class JSON_API FastWriter : public Writer {
public:
  FastWriter();
  virtual ~FastWriter() {}

  void enableYAMLCompatibility();

  /** \brief Drop the "null" string from the writer's output for nullValues.
   * Strictly speaking, this is not valid JSON. But when the output is being
   * fed to a browser's Javascript, it makes for smaller output and the
   * browser can handle the output just fine.
   */
  void dropNullPlaceholders();

  void omitEndingLineFeed();

public: // overridden from Writer
  virtual std::string write(const Value& root);

private:
  void writeValue(const Value& value);

  std::string document_;
  bool yamlCompatiblityEnabled_;
  bool dropNullPlaceholders_;
  bool omitEndingLineFeed_;
};

/** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a
 *human friendly way.
 *
 * The rules for line break and indent are as follow:
 * - Object value:
 *     - if empty then print {} without indent and line break
 *     - if not empty the print '{', line break & indent, print one value per
 *line
 *       and then unindent and line break and print '}'.
 * - Array value:
 *     - if empty then print [] without indent and line break
 *     - if the array contains no object value, empty array or some other value
 *types,
 *       and all the values fit on one lines, then print the array on a single
 *line.
 *     - otherwise, it the values do not fit on one line, or the array contains
 *       object or non empty array, then print one value per line.
 *
 * If the Value have comments then they are outputed according to their
 *#CommentPlacement.
 *
 * \sa Reader, Value, Value::setComment()
 */
class JSON_API StyledWriter : public Writer {
public:
  StyledWriter();
  virtual ~StyledWriter() {}

public: // overridden from Writer
  /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
   * \param root Value to serialize.
   * \return String containing the JSON document that represents the root value.
   */
  virtual std::string write(const Value& root);

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultineArray(const Value& value);
  void pushValue(const std::string& value);
  void writeIndent();
  void writeWithIndent(const std::string& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  bool hasCommentForValue(const Value& value);
  static std::string normalizeEOL(const std::string& text);

  typedef std::vector<std::string> ChildValues;

  ChildValues childValues_;
  std::string document_;
  std::string indentString_;
  int rightMargin_;
  int indentSize_;
  bool addChildValues_;
};

/** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a
 human friendly way,
	 to a stream rather than to a string.
 *
 * The rules for line break and indent are as follow:
 * - Object value:
 *     - if empty then print {} without indent and line break
 *     - if not empty the print '{', line break & indent, print one value per
 line
 *       and then unindent and line break and print '}'.
 * - Array value:
 *     - if empty then print [] without indent and line break
 *     - if the array contains no object value, empty array or some other value
 types,
 *       and all the values fit on one lines, then print the array on a single
 line.
 *     - otherwise, it the values do not fit on one line, or the array contains
 *       object or non empty array, then print one value per line.
 *
 * If the Value have comments then they are outputed according to their
 #CommentPlacement.
 *
 * \param indentation Each level will be indented by this amount extra.
 * \sa Reader, Value, Value::setComment()
 */
class JSON_API StyledStreamWriter {
public:
  StyledStreamWriter(std::string indentation = "\t");
  ~StyledStreamWriter() {}

public:
  /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
   * \param out Stream to write to. (Can be ostringstream, e.g.)
   * \param root Value to serialize.
   * \note There is no point in deriving from Writer, since write() should not
   * return a value.
   */
  void write(std::ostream& out, const Value& root);

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultineArray(const Value& value);
  void pushValue(const std::string& value);
  void writeIndent();
  void writeWithIndent(const std::string& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  bool hasCommentForValue(const Value& value);
  static std::string normalizeEOL(const std::string& text);

  typedef std::vector<std::string> ChildValues;

  ChildValues childValues_;
  std::ostream* document_;
  std::string indentString_;
  int rightMargin_;
  std::string indentation_;
  bool addChildValues_;
};

#if defined(JSON_HAS_INT64)
std::string JSON_API valueToString(Int value);
std::string JSON_API valueToString(UInt value);
#endif // if defined(JSON_HAS_INT64)
std::string JSON_API valueToString(LargestInt value);
std::string JSON_API valueToString(LargestUInt value);
std::string JSON_API valueToString(double value);
std::string JSON_API valueToString(bool value);
std::string JSON_API valueToQuotedString(const char* value);

/// \brief Output using the StyledStreamWriter.
/// \see Json::operator>>()
JSON_API std::ostream& operator<<(std::ostream&, const Value& root);

} // namespace Json

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(pop)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

#endif // JSON_WRITER_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/writer.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/assertions.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef CPPTL_JSON_ASSERTIONS_H_INCLUDED
#define CPPTL_JSON_ASSERTIONS_H_INCLUDED

#include <stdlib.h>

#if !defined(JSON_IS_AMALGAMATION)
#include "config.h"
#endif // if !defined(JSON_IS_AMALGAMATION)

#if JSON_USE_EXCEPTION
#include <stdexcept>
#define JSON_ASSERT(condition)                                                 \
  assert(condition); // @todo <= change this into an exception throw
#define JSON_FAIL_MESSAGE(message) throw std::runtime_error(message);
#else // JSON_USE_EXCEPTION
#define JSON_ASSERT(condition) assert(condition);

// The call to assert() will show the failure message in debug builds. In
// release bugs we write to invalid memory in order to crash hard, so that a
// debugger or crash reporter gets the chance to take over. We still call exit()
// afterward in order to tell the compiler that this macro doesn't return.
#define JSON_FAIL_MESSAGE(message)                                             \
  {                                                                            \
	assert(false&& message);                                                   \
	strcpy(reinterpret_cast<char*>(666), message);                             \
	exit(123);                                                                 \
  }

#endif

#define JSON_ASSERT_MESSAGE(condition, message)                                \
  if (!(condition)) {                                                          \
	JSON_FAIL_MESSAGE(message)                                                 \
  }

#endif // CPPTL_JSON_ASSERTIONS_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/assertions.h
// //////////////////////////////////////////////////////////////////////

#endif //ifndef JSON_AMALGATED_H_INCLUDED

#ifndef INCLUDE_CPP
#define INCLUDE_CPP

/*** Start of inlined file: jsoncpp.cpp ***/
/// Json-cpp amalgated source (http://jsoncpp.sourceforge.net/).
/// It is intented to be used with #include <json/json.h>

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////

/*
The JsonCpp library's source code, including accompanying documentation,
tests and demonstration applications, are licensed under the following
conditions...

The author (Baptiste Lepilleur) explicitly disclaims copyright in all
jurisdictions which recognize such a disclaimer. In such jurisdictions,
this software is released into the Public Domain.

In jurisdictions which do not recognize Public Domain property (e.g. Germany as of
2010), this software is Copyright (c) 2007-2010 by Baptiste Lepilleur, and is
released under the terms of the MIT License (see below).

In jurisdictions which recognize Public Domain property, the user of this
software may choose to accept it either as 1) Public Domain, 2) under the
conditions of the MIT License (see below), or 3) under the terms of dual
Public Domain/MIT License conditions described here, as they choose.

The MIT License is about as close to Public Domain as a license can get, and is
described in clear, concise terms at:

   http://en.wikipedia.org/wiki/MIT_License

The full text of the MIT License follows:

========================================================================
Copyright (c) 2007-2010 Baptiste Lepilleur

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
========================================================================
(END LICENSE TEXT)

The MIT license is compatible with both the GPL and commercial
software, affording one all of the rights of Public Domain with the
minor nuisance of being required to keep the above copyright notice
and license text in the source code. Note also that by accepting the
Public Domain "license" you can re-license your copy using whatever
license you like.

*/

// //////////////////////////////////////////////////////////////////////
// End of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_tool.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef LIB_JSONCPP_JSON_TOOL_H_INCLUDED
#define LIB_JSONCPP_JSON_TOOL_H_INCLUDED

/* This header provides common string manipulation support, such as UTF-8,
 * portable conversion from/to string...
 *
 * It is an internal header that must not be exposed.
 */

namespace Json {

/// Converts a unicode code-point to UTF-8.
static inline std::string codePointToUTF8(unsigned int cp) {
  std::string result;

  // based on description from http://en.wikipedia.org/wiki/UTF-8

  if (cp <= 0x7f) {
	result.resize(1);
	result[0] = static_cast<char>(cp);
  } else if (cp <= 0x7FF) {
	result.resize(2);
	result[1] = static_cast<char>(0x80 | (0x3f & cp));
	result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
  } else if (cp <= 0xFFFF) {
	result.resize(3);
	result[2] = static_cast<char>(0x80 | (0x3f & cp));
	result[1] = 0x80 | static_cast<char>((0x3f & (cp >> 6)));
	result[0] = 0xE0 | static_cast<char>((0xf & (cp >> 12)));
  } else if (cp <= 0x10FFFF) {
	result.resize(4);
	result[3] = static_cast<char>(0x80 | (0x3f & cp));
	result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
	result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
	result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
  }

  return result;
}

/// Returns true if ch is a control character (in range [0,32[).
static inline bool isControlCharacter(char ch) { return ch > 0 && ch <= 0x1F; }

enum {
  /// Constant that specify the size of the buffer that must be passed to
  /// uintToString.
  uintToStringBufferSize = 3 * sizeof(LargestUInt) + 1
};

// Defines a char buffer for use with uintToString().
typedef char UIntToStringBuffer[uintToStringBufferSize];

/** Converts an unsigned integer to string.
 * @param value Unsigned interger to convert to string
 * @param current Input/Output string buffer.
 *        Must have at least uintToStringBufferSize chars free.
 */
static inline void uintToString(LargestUInt value, char*& current) {
  *--current = 0;
  do {
	*--current = char(value % 10) + '0';
	value /= 10;
  } while (value != 0);
}

/** Change ',' to '.' everywhere in buffer.
 *
 * We had a sophisticated way, but it did not work in WinCE.
 * @see https://github.com/open-source-parsers/jsoncpp/pull/9
 */
static inline void fixNumericLocale(char* begin, char* end) {
  while (begin < end) {
	if (*begin == ',') {
	  *begin = '.';
	}
	++begin;
  }
}

} // namespace Json {

#endif // LIB_JSONCPP_JSON_TOOL_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_tool.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_reader.cpp
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2011 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#if !defined(JSON_IS_AMALGAMATION)
#include <json/assertions.h>
#include <json/reader.h>
#include <json/value.h>
#include "json_tool.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <utility>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <istream>

#if defined(_MSC_VER) && _MSC_VER < 1500 // VC++ 8.0 and below
#define snprintf _snprintf
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC++ 8.0
// Disable warning about strdup being deprecated.
#pragma warning(disable : 4996)
#endif

namespace Json {

// Implementation of class Features
// ////////////////////////////////

Features::Features()
	: allowComments_(true), strictRoot_(false),
	  allowDroppedNullPlaceholders_(false), allowNumericKeys_(false) {}

Features Features::all() { return Features(); }

Features Features::strictMode() {
  Features features;
  features.allowComments_ = false;
  features.strictRoot_ = true;
  features.allowDroppedNullPlaceholders_ = false;
  features.allowNumericKeys_ = false;
  return features;
}

// Implementation of class Reader
// ////////////////////////////////

static inline bool in(Reader::Char c,
					  Reader::Char c1,
					  Reader::Char c2,
					  Reader::Char c3,
					  Reader::Char c4) {
  return c == c1 || c == c2 || c == c3 || c == c4;
}

static inline bool in(Reader::Char c,
					  Reader::Char c1,
					  Reader::Char c2,
					  Reader::Char c3,
					  Reader::Char c4,
					  Reader::Char c5) {
  return c == c1 || c == c2 || c == c3 || c == c4 || c == c5;
}

static bool containsNewLine(Reader::Location begin, Reader::Location end) {
  for (; begin < end; ++begin)
	if (*begin == '\n' || *begin == '\r')
	  return true;
  return false;
}

// Class Reader
// //////////////////////////////////////////////////////////////////

Reader::Reader()
	: errors_(), document_(), begin_(), end_(), current_(), lastValueEnd_(),
	  lastValue_(), commentsBefore_(), features_(Features::all()),
	  collectComments_() {}

Reader::Reader(const Features& features)
	: errors_(), document_(), begin_(), end_(), current_(), lastValueEnd_(),
	  lastValue_(), commentsBefore_(), features_(features), collectComments_() {
}

bool
Reader::parse(const std::string& document, Value& root, bool collectComments) {
  document_ = document;
  const char* begin = document_.c_str();
  const char* end = begin + document_.length();
  return parse(begin, end, root, collectComments);
}

bool Reader::parse(std::istream& sin, Value& root, bool collectComments) {
  // std::istream_iterator<char> begin(sin);
  // std::istream_iterator<char> end;
  // Those would allow streamed input from a file, if parse() were a
  // template function.

  // Since std::string is reference-counted, this at least does not
  // create an extra copy.
  std::string doc;
  std::getline(sin, doc, (char)EOF);
  return parse(doc, root, collectComments);
}

bool Reader::parse(const char* beginDoc,
				   const char* endDoc,
				   Value& root,
				   bool collectComments) {
  if (!features_.allowComments_) {
	collectComments = false;
  }

  begin_ = beginDoc;
  end_ = endDoc;
  collectComments_ = collectComments;
  current_ = begin_;
  lastValueEnd_ = 0;
  lastValue_ = 0;
  commentsBefore_ = "";
  errors_.clear();
  while (!nodes_.empty())
	nodes_.pop();
  nodes_.push(&root);

  bool successful = readValue();
  Token token;
  skipCommentTokens(token);
  if (collectComments_ && !commentsBefore_.empty())
	root.setComment(commentsBefore_, commentAfter);
  if (features_.strictRoot_) {
	if (!root.isArray() && !root.isObject()) {
	  // Set error location to start of doc, ideally should be first token found
	  // in doc
	  token.type_ = tokenError;
	  token.start_ = beginDoc;
	  token.end_ = endDoc;
	  addError(
		  "A valid JSON document must be either an array or an object value.",
		  token);
	  return false;
	}
  }
  return successful;
}

bool Reader::readValue() {
  Token token;
  skipCommentTokens(token);
  bool successful = true;

  if (collectComments_ && !commentsBefore_.empty()) {
	// Remove newline characters at the end of the comments
	size_t lastNonNewline = commentsBefore_.find_last_not_of("\r\n");
	if (lastNonNewline != std::string::npos) {
	  commentsBefore_.erase(lastNonNewline + 1);
	} else {
	  commentsBefore_.clear();
	}

	currentValue().setComment(commentsBefore_, commentBefore);
	commentsBefore_ = "";
  }

  switch (token.type_) {
  case tokenObjectBegin:
	successful = readObject(token);
	currentValue().setOffsetLimit(current_ - begin_);
	break;
  case tokenArrayBegin:
	successful = readArray(token);
	currentValue().setOffsetLimit(current_ - begin_);
	break;
  case tokenNumber:
	successful = decodeNumber(token);
	break;
  case tokenString:
	successful = decodeString(token);
	break;
  case tokenTrue:
	currentValue() = true;
	currentValue().setOffsetStart(token.start_ - begin_);
	currentValue().setOffsetLimit(token.end_ - begin_);
	break;
  case tokenFalse:
	currentValue() = false;
	currentValue().setOffsetStart(token.start_ - begin_);
	currentValue().setOffsetLimit(token.end_ - begin_);
	break;
  case tokenNull:
	currentValue() = Value();
	currentValue().setOffsetStart(token.start_ - begin_);
	currentValue().setOffsetLimit(token.end_ - begin_);
	break;
  case tokenArraySeparator:
	if (features_.allowDroppedNullPlaceholders_) {
	  // "Un-read" the current token and mark the current value as a null
	  // token.
	  current_--;
	  currentValue() = Value();
	  currentValue().setOffsetStart(current_ - begin_ - 1);
	  currentValue().setOffsetLimit(current_ - begin_);
	  break;
	}
  // Else, fall through...
  default:
	currentValue().setOffsetStart(token.start_ - begin_);
	currentValue().setOffsetLimit(token.end_ - begin_);
	return addError("Syntax error: value, object or array expected.", token);
  }

  if (collectComments_) {
	lastValueEnd_ = current_;
	lastValue_ = &currentValue();
  }

  return successful;
}

void Reader::skipCommentTokens(Token& token) {
  if (features_.allowComments_) {
	do {
	  readToken(token);
	} while (token.type_ == tokenComment);
  } else {
	readToken(token);
  }
}

bool Reader::expectToken(TokenType type, Token& token, const char* message) {
  readToken(token);
  if (token.type_ != type)
	return addError(message, token);
  return true;
}

bool Reader::readToken(Token& token) {
  skipSpaces();
  token.start_ = current_;
  Char c = getNextChar();
  bool ok = true;
  switch (c) {
  case '{':
	token.type_ = tokenObjectBegin;
	break;
  case '}':
	token.type_ = tokenObjectEnd;
	break;
  case '[':
	token.type_ = tokenArrayBegin;
	break;
  case ']':
	token.type_ = tokenArrayEnd;
	break;
  case '"':
	token.type_ = tokenString;
	ok = readString();
	break;
  case '/':
	token.type_ = tokenComment;
	ok = readComment();
	break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '-':
	token.type_ = tokenNumber;
	readNumber();
	break;
  case 't':
	token.type_ = tokenTrue;
	ok = match("rue", 3);
	break;
  case 'f':
	token.type_ = tokenFalse;
	ok = match("alse", 4);
	break;
  case 'n':
	token.type_ = tokenNull;
	ok = match("ull", 3);
	break;
  case ',':
	token.type_ = tokenArraySeparator;
	break;
  case ':':
	token.type_ = tokenMemberSeparator;
	break;
  case 0:
	token.type_ = tokenEndOfStream;
	break;
  default:
	ok = false;
	break;
  }
  if (!ok)
	token.type_ = tokenError;
  token.end_ = current_;
  return true;
}

void Reader::skipSpaces() {
  while (current_ != end_) {
	Char c = *current_;
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
	  ++current_;
	else
	  break;
  }
}

bool Reader::match(Location pattern, int patternLength) {
  if (end_ - current_ < patternLength)
	return false;
  int index = patternLength;
  while (index--)
	if (current_[index] != pattern[index])
	  return false;
  current_ += patternLength;
  return true;
}

bool Reader::readComment() {
  Location commentBegin = current_ - 1;
  Char c = getNextChar();
  bool successful = false;
  if (c == '*')
	successful = readCStyleComment();
  else if (c == '/')
	successful = readCppStyleComment();
  if (!successful)
	return false;

  if (collectComments_) {
	CommentPlacement placement = commentBefore;
	if (lastValueEnd_ && !containsNewLine(lastValueEnd_, commentBegin)) {
	  if (c != '*' || !containsNewLine(commentBegin, current_))
		placement = commentAfterOnSameLine;
	}

	addComment(commentBegin, current_, placement);
  }
  return true;
}

void
Reader::addComment(Location begin, Location end, CommentPlacement placement) {
  assert(collectComments_);
  if (placement == commentAfterOnSameLine) {
	assert(lastValue_ != 0);
	lastValue_->setComment(std::string(begin, end), placement);
  } else {
	commentsBefore_ += std::string(begin, end);
  }
}

bool Reader::readCStyleComment() {
  while (current_ != end_) {
	Char c = getNextChar();
	if (c == '*' && *current_ == '/')
	  break;
  }
  return getNextChar() == '/';
}

bool Reader::readCppStyleComment() {
  while (current_ != end_) {
	Char c = getNextChar();
	if (c == '\r' || c == '\n')
	  break;
  }
  return true;
}

void Reader::readNumber() {
  while (current_ != end_) {
	if (!(*current_ >= '0' && *current_ <= '9') &&
		!in(*current_, '.', 'e', 'E', '+', '-'))
	  break;
	++current_;
  }
}

bool Reader::readString() {
  Char c = 0;
  while (current_ != end_) {
	c = getNextChar();
	if (c == '\\')
	  getNextChar();
	else if (c == '"')
	  break;
  }
  return c == '"';
}

bool Reader::readObject(Token& tokenStart) {
  Token tokenName;
  std::string name;
  currentValue() = Value(objectValue);
  currentValue().setOffsetStart(tokenStart.start_ - begin_);
  while (readToken(tokenName)) {
	bool initialTokenOk = true;
	while (tokenName.type_ == tokenComment && initialTokenOk)
	  initialTokenOk = readToken(tokenName);
	if (!initialTokenOk)
	  break;
	if (tokenName.type_ == tokenObjectEnd && name.empty()) // empty object
	  return true;
	name = "";
	if (tokenName.type_ == tokenString) {
	  if (!decodeString(tokenName, name))
		return recoverFromError(tokenObjectEnd);
	} else if (tokenName.type_ == tokenNumber && features_.allowNumericKeys_) {
	  Value numberName;
	  if (!decodeNumber(tokenName, numberName))
		return recoverFromError(tokenObjectEnd);
	  name = numberName.asString();
	} else {
	  break;
	}

	Token colon;
	if (!readToken(colon) || colon.type_ != tokenMemberSeparator) {
	  return addErrorAndRecover(
		  "Missing ':' after object member name", colon, tokenObjectEnd);
	}
	Value& value = currentValue()[name];
	nodes_.push(&value);
	bool ok = readValue();
	nodes_.pop();
	if (!ok) // error already set
	  return recoverFromError(tokenObjectEnd);

	Token comma;
	if (!readToken(comma) ||
		(comma.type_ != tokenObjectEnd && comma.type_ != tokenArraySeparator &&
		 comma.type_ != tokenComment)) {
	  return addErrorAndRecover(
		  "Missing ',' or '}' in object declaration", comma, tokenObjectEnd);
	}
	bool finalizeTokenOk = true;
	while (comma.type_ == tokenComment && finalizeTokenOk)
	  finalizeTokenOk = readToken(comma);
	if (comma.type_ == tokenObjectEnd)
	  return true;
  }
  return addErrorAndRecover(
	  "Missing '}' or object member name", tokenName, tokenObjectEnd);
}

bool Reader::readArray(Token& tokenStart) {
  currentValue() = Value(arrayValue);
  currentValue().setOffsetStart(tokenStart.start_ - begin_);
  skipSpaces();
  if (*current_ == ']') // empty array
  {
	Token endArray;
	readToken(endArray);
	return true;
  }
  int index = 0;
  for (;;) {
	Value& value = currentValue()[index++];
	nodes_.push(&value);
	bool ok = readValue();
	nodes_.pop();
	if (!ok) // error already set
	  return recoverFromError(tokenArrayEnd);

	Token token;
	// Accept Comment after last item in the array.
	ok = readToken(token);
	while (token.type_ == tokenComment && ok) {
	  ok = readToken(token);
	}
	bool badTokenType =
		(token.type_ != tokenArraySeparator && token.type_ != tokenArrayEnd);
	if (!ok || badTokenType) {
	  return addErrorAndRecover(
		  "Missing ',' or ']' in array declaration", token, tokenArrayEnd);
	}
	if (token.type_ == tokenArrayEnd)
	  break;
  }
  return true;
}

bool Reader::decodeNumber(Token& token) {
  Value decoded;
  if (!decodeNumber(token, decoded))
	return false;
  currentValue() = decoded;
  currentValue().setOffsetStart(token.start_ - begin_);
  currentValue().setOffsetLimit(token.end_ - begin_);
  return true;
}

bool Reader::decodeNumber(Token& token, Value& decoded) {
  bool isDouble = false;
  for (Location inspect = token.start_; inspect != token.end_; ++inspect) {
	isDouble = isDouble || in(*inspect, '.', 'e', 'E', '+') ||
			   (*inspect == '-' && inspect != token.start_);
  }
  if (isDouble)
	return decodeDouble(token, decoded);
  // Attempts to parse the number as an integer. If the number is
  // larger than the maximum supported value of an integer then
  // we decode the number as a double.
  Location current = token.start_;
  bool isNegative = *current == '-';
  if (isNegative)
	++current;
  Value::LargestUInt maxIntegerValue =
	  isNegative ? Value::LargestUInt(-Value::minLargestInt)
				 : Value::maxLargestUInt;
  Value::LargestUInt threshold = maxIntegerValue / 10;
  Value::LargestUInt value = 0;
  while (current < token.end_) {
	Char c = *current++;
	if (c < '0' || c > '9')
	  return addError("'" + std::string(token.start_, token.end_) +
						  "' is not a number.",
					  token);
	Value::UInt digit(c - '0');
	if (value >= threshold) {
	  // We've hit or exceeded the max value divided by 10 (rounded down). If
	  // a) we've only just touched the limit, b) this is the last digit, and
	  // c) it's small enough to fit in that rounding delta, we're okay.
	  // Otherwise treat this number as a double to avoid overflow.
	  if (value > threshold || current != token.end_ ||
		  digit > maxIntegerValue % 10) {
		return decodeDouble(token, decoded);
	  }
	}
	value = value * 10 + digit;
  }
  if (isNegative)
	decoded = -Value::LargestInt(value);
  else if (value <= Value::LargestUInt(Value::maxInt))
	decoded = Value::LargestInt(value);
  else
	decoded = value;
  return true;
}

bool Reader::decodeDouble(Token& token) {
  Value decoded;
  if (!decodeDouble(token, decoded))
	return false;
  currentValue() = decoded;
  currentValue().setOffsetStart(token.start_ - begin_);
  currentValue().setOffsetLimit(token.end_ - begin_);
  return true;
}

bool Reader::decodeDouble(Token& token, Value& decoded) {
  double value = 0;
  const int bufferSize = 32;
  int count;
  int length = int(token.end_ - token.start_);

  // Sanity check to avoid buffer overflow exploits.
  if (length < 0) {
	return addError("Unable to parse token length", token);
  }

  // Avoid using a string constant for the format control string given to
  // sscanf, as this can cause hard to debug crashes on OS X. See here for more
  // info:
  //
  //     http://developer.apple.com/library/mac/#DOCUMENTATION/DeveloperTools/gcc-4.0.1/gcc/Incompatibilities.html
  char format[] = "%lf";

  if (length <= bufferSize) {
	Char buffer[bufferSize + 1];
	memcpy(buffer, token.start_, length);
	buffer[length] = 0;
	count = sscanf(buffer, format, &value);
  } else {
	std::string buffer(token.start_, token.end_);
	count = sscanf(buffer.c_str(), format, &value);
  }

  if (count != 1)
	return addError("'" + std::string(token.start_, token.end_) +
						"' is not a number.",
					token);
  decoded = value;
  return true;
}

bool Reader::decodeString(Token& token) {
  std::string decoded;
  if (!decodeString(token, decoded))
	return false;
  currentValue() = decoded;
  currentValue().setOffsetStart(token.start_ - begin_);
  currentValue().setOffsetLimit(token.end_ - begin_);
  return true;
}

bool Reader::decodeString(Token& token, std::string& decoded) {
  decoded.reserve(token.end_ - token.start_ - 2);
  Location current = token.start_ + 1; // skip '"'
  Location end = token.end_ - 1;       // do not include '"'
  while (current != end) {
	Char c = *current++;
	if (c == '"')
	  break;
	else if (c == '\\') {
	  if (current == end)
		return addError("Empty escape sequence in string", token, current);
	  Char escape = *current++;
	  switch (escape) {
	  case '"':
		decoded += '"';
		break;
	  case '/':
		decoded += '/';
		break;
	  case '\\':
		decoded += '\\';
		break;
	  case 'b':
		decoded += '\b';
		break;
	  case 'f':
		decoded += '\f';
		break;
	  case 'n':
		decoded += '\n';
		break;
	  case 'r':
		decoded += '\r';
		break;
	  case 't':
		decoded += '\t';
		break;
	  case 'u': {
		unsigned int unicode;
		if (!decodeUnicodeCodePoint(token, current, end, unicode))
		  return false;
		decoded += codePointToUTF8(unicode);
	  } break;
	  default:
		return addError("Bad escape sequence in string", token, current);
	  }
	} else {
	  decoded += c;
	}
  }
  return true;
}

bool Reader::decodeUnicodeCodePoint(Token& token,
									Location& current,
									Location end,
									unsigned int& unicode) {

  if (!decodeUnicodeEscapeSequence(token, current, end, unicode))
	return false;
  if (unicode >= 0xD800 && unicode <= 0xDBFF) {
	// surrogate pairs
	if (end - current < 6)
	  return addError(
		  "additional six characters expected to parse unicode surrogate pair.",
		  token,
		  current);
	unsigned int surrogatePair;
	if (*(current++) == '\\' && *(current++) == 'u') {
	  if (decodeUnicodeEscapeSequence(token, current, end, surrogatePair)) {
		unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
	  } else
		return false;
	} else
	  return addError("expecting another \\u token to begin the second half of "
					  "a unicode surrogate pair",
					  token,
					  current);
  }
  return true;
}

bool Reader::decodeUnicodeEscapeSequence(Token& token,
										 Location& current,
										 Location end,
										 unsigned int& unicode) {
  if (end - current < 4)
	return addError(
		"Bad unicode escape sequence in string: four digits expected.",
		token,
		current);
  unicode = 0;
  for (int index = 0; index < 4; ++index) {
	Char c = *current++;
	unicode *= 16;
	if (c >= '0' && c <= '9')
	  unicode += c - '0';
	else if (c >= 'a' && c <= 'f')
	  unicode += c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
	  unicode += c - 'A' + 10;
	else
	  return addError(
		  "Bad unicode escape sequence in string: hexadecimal digit expected.",
		  token,
		  current);
  }
  return true;
}

bool
Reader::addError(const std::string& message, Token& token, Location extra) {
  ErrorInfo info;
  info.token_ = token;
  info.message_ = message;
  info.extra_ = extra;
  errors_.push_back(info);
  return false;
}

bool Reader::recoverFromError(TokenType skipUntilToken) {
  int errorCount = int(errors_.size());
  Token skip;
  for (;;) {
	if (!readToken(skip))
	  errors_.resize(errorCount); // discard errors caused by recovery
	if (skip.type_ == skipUntilToken || skip.type_ == tokenEndOfStream)
	  break;
  }
  errors_.resize(errorCount);
  return false;
}

bool Reader::addErrorAndRecover(const std::string& message,
								Token& token,
								TokenType skipUntilToken) {
  addError(message, token);
  return recoverFromError(skipUntilToken);
}

Value& Reader::currentValue() { return *(nodes_.top()); }

Reader::Char Reader::getNextChar() {
  if (current_ == end_)
	return 0;
  return *current_++;
}

void Reader::getLocationLineAndColumn(Location location,
									  int& line,
									  int& column) const {
  Location current = begin_;
  Location lastLineStart = current;
  line = 0;
  while (current < location && current != end_) {
	Char c = *current++;
	if (c == '\r') {
	  if (*current == '\n')
		++current;
	  lastLineStart = current;
	  ++line;
	} else if (c == '\n') {
	  lastLineStart = current;
	  ++line;
	}
  }
  // column & line start at 1
  column = int(location - lastLineStart) + 1;
  ++line;
}

std::string Reader::getLocationLineAndColumn(Location location) const {
  int line, column;
  getLocationLineAndColumn(location, line, column);
  char buffer[18 + 16 + 16 + 1];
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__)
#if defined(WINCE)
  _snprintf(buffer, sizeof(buffer), "Line %d, Column %d", line, column);
#else
  sprintf_s(buffer, sizeof(buffer), "Line %d, Column %d", line, column);
#endif
#else
  snprintf(buffer, sizeof(buffer), "Line %d, Column %d", line, column);
#endif
  return buffer;
}

// Deprecated. Preserved for backward compatibility
std::string Reader::getFormatedErrorMessages() const {
  return getFormattedErrorMessages();
}

std::string Reader::getFormattedErrorMessages() const {
  std::string formattedMessage;
  for (Errors::const_iterator itError = errors_.begin();
	   itError != errors_.end();
	   ++itError) {
	const ErrorInfo& error = *itError;
	formattedMessage +=
		"* " + getLocationLineAndColumn(error.token_.start_) + "\n";
	formattedMessage += "  " + error.message_ + "\n";
	if (error.extra_)
	  formattedMessage +=
		  "See " + getLocationLineAndColumn(error.extra_) + " for detail.\n";
  }
  return formattedMessage;
}

std::vector<Reader::StructuredError> Reader::getStructuredErrors() const {
  std::vector<Reader::StructuredError> allErrors;
  for (Errors::const_iterator itError = errors_.begin();
	   itError != errors_.end();
	   ++itError) {
	const ErrorInfo& error = *itError;
	Reader::StructuredError structured;
	structured.offset_start = error.token_.start_ - begin_;
	structured.offset_limit = error.token_.end_ - begin_;
	structured.message = error.message_;
	allErrors.push_back(structured);
  }
  return allErrors;
}

std::istream& operator>>(std::istream& sin, Value& root) {
  Json::Reader reader;
  bool ok = reader.parse(sin, root, true);
  if (!ok) {
	fprintf(stderr,
			"Error from reader: %s",
			reader.getFormattedErrorMessages().c_str());

	JSON_FAIL_MESSAGE("reader error");
  }
  return sin;
}

} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_reader.cpp
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_batchallocator.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSONCPP_BATCHALLOCATOR_H_INCLUDED
#define JSONCPP_BATCHALLOCATOR_H_INCLUDED

#include <stdlib.h>
#include <assert.h>

#ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

namespace Json {

/* Fast memory allocator.
 *
 * This memory allocator allocates memory for a batch of object (specified by
 * the page size, the number of object in each page).
 *
 * It does not allow the destruction of a single object. All the allocated
 * objects can be destroyed at once. The memory can be either released or reused
 * for future allocation.
 *
 * The in-place new operator must be used to construct the object using the
 * pointer returned by allocate.
 */
template <typename AllocatedType, const unsigned int objectPerAllocation>
class BatchAllocator {
public:
  BatchAllocator(unsigned int objectsPerPage = 255)
	  : freeHead_(0), objectsPerPage_(objectsPerPage) {
	//      printf( "Size: %d => %s\n", sizeof(AllocatedType),
	// typeid(AllocatedType).name() );
	assert(sizeof(AllocatedType) * objectPerAllocation >=
		   sizeof(AllocatedType*)); // We must be able to store a slist in the
									// object free space.
	assert(objectsPerPage >= 16);
	batches_ = allocateBatch(0); // allocated a dummy page
	currentBatch_ = batches_;
  }

  ~BatchAllocator() {
	for (BatchInfo* batch = batches_; batch;) {
	  BatchInfo* nextBatch = batch->next_;
	  free(batch);
	  batch = nextBatch;
	}
  }

  /// allocate space for an array of objectPerAllocation object.
  /// @warning it is the responsability of the caller to call objects
  /// constructors.
  AllocatedType* allocate() {
	if (freeHead_) // returns node from free list.
	{
	  AllocatedType* object = freeHead_;
	  freeHead_ = *(AllocatedType**)object;
	  return object;
	}
	if (currentBatch_->used_ == currentBatch_->end_) {
	  currentBatch_ = currentBatch_->next_;
	  while (currentBatch_ && currentBatch_->used_ == currentBatch_->end_)
		currentBatch_ = currentBatch_->next_;

	  if (!currentBatch_) // no free batch found, allocate a new one
	  {
		currentBatch_ = allocateBatch(objectsPerPage_);
		currentBatch_->next_ = batches_; // insert at the head of the list
		batches_ = currentBatch_;
	  }
	}
	AllocatedType* allocated = currentBatch_->used_;
	currentBatch_->used_ += objectPerAllocation;
	return allocated;
  }

  /// Release the object.
  /// @warning it is the responsability of the caller to actually destruct the
  /// object.
  void release(AllocatedType* object) {
	assert(object != 0);
	*(AllocatedType**)object = freeHead_;
	freeHead_ = object;
  }

private:
  struct BatchInfo {
	BatchInfo* next_;
	AllocatedType* used_;
	AllocatedType* end_;
	AllocatedType buffer_[objectPerAllocation];
  };

  // disabled copy constructor and assignement operator.
  BatchAllocator(const BatchAllocator&);
  void operator=(const BatchAllocator&);

  static BatchInfo* allocateBatch(unsigned int objectsPerPage) {
	const unsigned int mallocSize =
		sizeof(BatchInfo) - sizeof(AllocatedType) * objectPerAllocation +
		sizeof(AllocatedType) * objectPerAllocation * objectsPerPage;
	BatchInfo* batch = static_cast<BatchInfo*>(malloc(mallocSize));
	batch->next_ = 0;
	batch->used_ = batch->buffer_;
	batch->end_ = batch->buffer_ + objectsPerPage;
	return batch;
  }

  BatchInfo* batches_;
  BatchInfo* currentBatch_;
  /// Head of a single linked list within the allocated space of freeed object
  AllocatedType* freeHead_;
  unsigned int objectsPerPage_;
};

} // namespace Json

#endif // ifndef JSONCPP_DOC_INCLUDE_IMPLEMENTATION

#endif // JSONCPP_BATCHALLOCATOR_H_INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_batchallocator.h
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_valueiterator.inl
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

// included by json_value.cpp

namespace Json {

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueIteratorBase
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueIteratorBase::ValueIteratorBase()
#ifndef JSON_VALUE_USE_INTERNAL_MAP
	: current_(), isNull_(true) {
}
#else
	: isArray_(true), isNull_(true) {
  iterator_.array_ = ValueInternalArray::IteratorState();
}
#endif

#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueIteratorBase::ValueIteratorBase(
	const Value::ObjectValues::iterator& current)
	: current_(current), isNull_(false) {}
#else
ValueIteratorBase::ValueIteratorBase(
	const ValueInternalArray::IteratorState& state)
	: isArray_(true) {
  iterator_.array_ = state;
}

ValueIteratorBase::ValueIteratorBase(
	const ValueInternalMap::IteratorState& state)
	: isArray_(false) {
  iterator_.map_ = state;
}
#endif

Value& ValueIteratorBase::deref() const {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  return current_->second;
#else
  if (isArray_)
	return ValueInternalArray::dereference(iterator_.array_);
  return ValueInternalMap::value(iterator_.map_);
#endif
}

void ValueIteratorBase::increment() {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  ++current_;
#else
  if (isArray_)
	ValueInternalArray::increment(iterator_.array_);
  ValueInternalMap::increment(iterator_.map_);
#endif
}

void ValueIteratorBase::decrement() {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  --current_;
#else
  if (isArray_)
	ValueInternalArray::decrement(iterator_.array_);
  ValueInternalMap::decrement(iterator_.map_);
#endif
}

ValueIteratorBase::difference_type
ValueIteratorBase::computeDistance(const SelfType& other) const {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
#ifdef JSON_USE_CPPTL_SMALLMAP
  return current_ - other.current_;
#else
  // Iterator for null value are initialized using the default
  // constructor, which initialize current_ to the default
  // std::map::iterator. As begin() and end() are two instance
  // of the default std::map::iterator, they can not be compared.
  // To allow this, we handle this comparison specifically.
  if (isNull_ && other.isNull_) {
	return 0;
  }

  // Usage of std::distance is not portable (does not compile with Sun Studio 12
  // RogueWave STL,
  // which is the one used by default).
  // Using a portable hand-made version for non random iterator instead:
  //   return difference_type( std::distance( current_, other.current_ ) );
  difference_type myDistance = 0;
  for (Value::ObjectValues::iterator it = current_; it != other.current_;
	   ++it) {
	++myDistance;
  }
  return myDistance;
#endif
#else
  if (isArray_)
	return ValueInternalArray::distance(iterator_.array_,
										other.iterator_.array_);
  return ValueInternalMap::distance(iterator_.map_, other.iterator_.map_);
#endif
}

bool ValueIteratorBase::isEqual(const SelfType& other) const {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  if (isNull_) {
	return other.isNull_;
  }
  return current_ == other.current_;
#else
  if (isArray_)
	return ValueInternalArray::equals(iterator_.array_, other.iterator_.array_);
  return ValueInternalMap::equals(iterator_.map_, other.iterator_.map_);
#endif
}

void ValueIteratorBase::copy(const SelfType& other) {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  current_ = other.current_;
  isNull_ = other.isNull_;
#else
  if (isArray_)
	iterator_.array_ = other.iterator_.array_;
  iterator_.map_ = other.iterator_.map_;
#endif
}

Value ValueIteratorBase::key() const {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  const Value::CZString czstring = (*current_).first;
  if (czstring.c_str()) {
	if (czstring.isStaticString())
	  return Value(StaticString(czstring.c_str()));
	return Value(czstring.c_str());
  }
  return Value(czstring.index());
#else
  if (isArray_)
	return Value(ValueInternalArray::indexOf(iterator_.array_));
  bool isStatic;
  const char* memberName = ValueInternalMap::key(iterator_.map_, isStatic);
  if (isStatic)
	return Value(StaticString(memberName));
  return Value(memberName);
#endif
}

UInt ValueIteratorBase::index() const {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  const Value::CZString czstring = (*current_).first;
  if (!czstring.c_str())
	return czstring.index();
  return Value::UInt(-1);
#else
  if (isArray_)
	return Value::UInt(ValueInternalArray::indexOf(iterator_.array_));
  return Value::UInt(-1);
#endif
}

const char* ValueIteratorBase::memberName() const {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  const char* name = (*current_).first.c_str();
  return name ? name : "";
#else
  if (!isArray_)
	return ValueInternalMap::key(iterator_.map_);
  return "";
#endif
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueConstIterator
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueConstIterator::ValueConstIterator() {}

#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueConstIterator::ValueConstIterator(
	const Value::ObjectValues::iterator& current)
	: ValueIteratorBase(current) {}
#else
ValueConstIterator::ValueConstIterator(
	const ValueInternalArray::IteratorState& state)
	: ValueIteratorBase(state) {}

ValueConstIterator::ValueConstIterator(
	const ValueInternalMap::IteratorState& state)
	: ValueIteratorBase(state) {}
#endif

ValueConstIterator& ValueConstIterator::
operator=(const ValueIteratorBase& other) {
  copy(other);
  return *this;
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class ValueIterator
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

ValueIterator::ValueIterator() {}

#ifndef JSON_VALUE_USE_INTERNAL_MAP
ValueIterator::ValueIterator(const Value::ObjectValues::iterator& current)
	: ValueIteratorBase(current) {}
#else
ValueIterator::ValueIterator(const ValueInternalArray::IteratorState& state)
	: ValueIteratorBase(state) {}

ValueIterator::ValueIterator(const ValueInternalMap::IteratorState& state)
	: ValueIteratorBase(state) {}
#endif

ValueIterator::ValueIterator(const ValueConstIterator& other)
	: ValueIteratorBase(other) {}

ValueIterator::ValueIterator(const ValueIterator& other)
	: ValueIteratorBase(other) {}

ValueIterator& ValueIterator::operator=(const SelfType& other) {
  copy(other);
  return *this;
}

} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_valueiterator.inl
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_value.cpp
// //////////////////////////////////////////////////////////////////////

// Copyright 2011 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#if !defined(JSON_IS_AMALGAMATION)
#include <json/assertions.h>
#include <json/value.h>
#include <json/writer.h>
#ifndef JSON_USE_SIMPLE_INTERNAL_ALLOCATOR
#include "json_batchallocator.h"
#endif // #ifndef JSON_USE_SIMPLE_INTERNAL_ALLOCATOR
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <math.h>
#include <sstream>
#include <utility>
#include <cstring>
#include <cassert>
#ifdef JSON_USE_CPPTL
#include <cpptl/conststring.h>
#endif
#include <cstddef> // size_t

#define JSON_ASSERT_UNREACHABLE assert(false)

namespace Json {

// This is a walkaround to avoid the static initialization of Value::null.
// kNull must be word-aligned to avoid crashing on ARM.  We use an alignment of
// 8 (instead of 4) as a bit of future-proofing.
#if defined(__ARMEL__)
#define ALIGNAS(byte_alignment) __attribute__((aligned(byte_alignment)))
#else
#define ALIGNAS(byte_alignment)
#endif
static const unsigned char ALIGNAS(8) kNull[sizeof(Value)] = { 0 };
const unsigned char& kNullRef = kNull[0];
const Value& Value::null = reinterpret_cast<const Value&>(kNullRef);

const Int Value::minInt = Int(~(UInt(-1) / 2));
const Int Value::maxInt = Int(UInt(-1) / 2);
const UInt Value::maxUInt = UInt(-1);
#if defined(JSON_HAS_INT64)
const Int64 Value::minInt64 = Int64(~(UInt64(-1) / 2));
const Int64 Value::maxInt64 = Int64(UInt64(-1) / 2);
const UInt64 Value::maxUInt64 = UInt64(-1);
// The constant is hard-coded because some compiler have trouble
// converting Value::maxUInt64 to a double correctly (AIX/xlC).
// Assumes that UInt64 is a 64 bits integer.
static const double maxUInt64AsDouble = 18446744073709551615.0;
#endif // defined(JSON_HAS_INT64)
const LargestInt Value::minLargestInt = LargestInt(~(LargestUInt(-1) / 2));
const LargestInt Value::maxLargestInt = LargestInt(LargestUInt(-1) / 2);
const LargestUInt Value::maxLargestUInt = LargestUInt(-1);

/// Unknown size marker
static const unsigned int unknown = (unsigned)-1;

#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
template <typename T, typename U>
static inline bool InRange(double d, T min, U max) {
  return d >= min && d <= max;
}
#else  // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
static inline double integerToDouble(Json::UInt64 value) {
  return static_cast<double>(Int64(value / 2)) * 2.0 + Int64(value & 1);
}

template <typename T> static inline double integerToDouble(T value) {
  return static_cast<double>(value);
}

template <typename T, typename U>
static inline bool InRange(double d, T min, U max) {
  return d >= integerToDouble(min) && d <= integerToDouble(max);
}
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)

/** Duplicates the specified string value.
 * @param value Pointer to the string to duplicate. Must be zero-terminated if
 *              length is "unknown".
 * @param length Length of the value. if equals to unknown, then it will be
 *               computed using strlen(value).
 * @return Pointer on the duplicate instance of string.
 */
static inline char* duplicateStringValue(const char* value,
										 unsigned int length = unknown) {
  if (length == unknown)
	length = (unsigned int)strlen(value);

  // Avoid an integer overflow in the call to malloc below by limiting length
  // to a sane value.
  if (length >= (unsigned)Value::maxInt)
	length = Value::maxInt - 1;

  char* newString = static_cast<char*>(malloc(length + 1));
  JSON_ASSERT_MESSAGE(newString != 0,
					  "in Json::Value::duplicateStringValue(): "
					  "Failed to allocate string value buffer");
  memcpy(newString, value, length);
  newString[length] = 0;
  return newString;
}

/** Free the string duplicated by duplicateStringValue().
 */
static inline void releaseStringValue(char* value) { free(value); }

} // namespace Json

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// ValueInternals...
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
#if !defined(JSON_IS_AMALGAMATION)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
#include "json_internalarray.inl"
#include "json_internalmap.inl"
#endif // JSON_VALUE_USE_INTERNAL_MAP

#include "json_valueiterator.inl"
#endif // if !defined(JSON_IS_AMALGAMATION)

namespace Json {

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::CommentInfo
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

Value::CommentInfo::CommentInfo() : comment_(0) {}

Value::CommentInfo::~CommentInfo() {
  if (comment_)
	releaseStringValue(comment_);
}

void Value::CommentInfo::setComment(const char* text) {
  if (comment_)
	releaseStringValue(comment_);
  JSON_ASSERT(text != 0);
  JSON_ASSERT_MESSAGE(
	  text[0] == '\0' || text[0] == '/',
	  "in Json::Value::setComment(): Comments must start with /");
  // It seems that /**/ style comments are acceptable as well.
  comment_ = duplicateStringValue(text);
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::CZString
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
#ifndef JSON_VALUE_USE_INTERNAL_MAP

// Notes: index_ indicates if the string was allocated when
// a string is stored.

Value::CZString::CZString(ArrayIndex index) : cstr_(0), index_(index) {}

Value::CZString::CZString(const char* cstr, DuplicationPolicy allocate)
	: cstr_(allocate == duplicate ? duplicateStringValue(cstr) : cstr),
	  index_(allocate) {}

Value::CZString::CZString(const CZString& other)
	: cstr_(other.index_ != noDuplication && other.cstr_ != 0
				? duplicateStringValue(other.cstr_)
				: other.cstr_),
	  index_(other.cstr_
				 ? (other.index_ == noDuplication ? noDuplication : duplicate)
				 : other.index_) {}

Value::CZString::~CZString() {
  if (cstr_ && index_ == duplicate)
	releaseStringValue(const_cast<char*>(cstr_));
}

void Value::CZString::swap(CZString& other) {
  std::swap(cstr_, other.cstr_);
  std::swap(index_, other.index_);
}

Value::CZString& Value::CZString::operator=(CZString other) {
  swap(other);
  return *this;
}

bool Value::CZString::operator<(const CZString& other) const {
  if (cstr_)
	return strcmp(cstr_, other.cstr_) < 0;
  return index_ < other.index_;
}

bool Value::CZString::operator==(const CZString& other) const {
  if (cstr_)
	return strcmp(cstr_, other.cstr_) == 0;
  return index_ == other.index_;
}

ArrayIndex Value::CZString::index() const { return index_; }

const char* Value::CZString::c_str() const { return cstr_; }

bool Value::CZString::isStaticString() const { return index_ == noDuplication; }

#endif // ifndef JSON_VALUE_USE_INTERNAL_MAP

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::Value
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

/*! \internal Default constructor initialization must be equivalent to:
 * memset( this, 0, sizeof(Value) )
 * This optimization is used in ValueInternalMap fast allocator.
 */
Value::Value(ValueType type)
	: type_(type), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  switch (type) {
  case nullValue:
	break;
  case intValue:
  case uintValue:
	value_.int_ = 0;
	break;
  case realValue:
	value_.real_ = 0.0;
	break;
  case stringValue:
	value_.string_ = 0;
	break;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
  case objectValue:
	value_.map_ = new ObjectValues();
	break;
#else
  case arrayValue:
	value_.array_ = arrayAllocator()->newArray();
	break;
  case objectValue:
	value_.map_ = mapAllocator()->newMap();
	break;
#endif
  case booleanValue:
	value_.bool_ = false;
	break;
  default:
	JSON_ASSERT_UNREACHABLE;
  }
}

Value::Value(UInt value)
	: type_(uintValue), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.uint_ = value;
}

Value::Value(Int value)
	: type_(intValue), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.int_ = value;
}

#if defined(JSON_HAS_INT64)
Value::Value(Int64 value)
	: type_(intValue), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.int_ = value;
}

Value::Value(UInt64 value)
	: type_(uintValue), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.uint_ = value;
}
#endif // defined(JSON_HAS_INT64)

Value::Value(double value)
	: type_(realValue), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.real_ = value;
}

Value::Value(const char* value)
	: type_(stringValue), allocated_(true)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.string_ = duplicateStringValue(value);
}

Value::Value(const char* beginValue, const char* endValue)
	: type_(stringValue), allocated_(true)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.string_ =
	  duplicateStringValue(beginValue, (unsigned int)(endValue - beginValue));
}

Value::Value(const std::string& value)
	: type_(stringValue), allocated_(true)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.string_ =
	  duplicateStringValue(value.c_str(), (unsigned int)value.length());
}

Value::Value(const StaticString& value)
	: type_(stringValue), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.string_ = const_cast<char*>(value.c_str());
}

#ifdef JSON_USE_CPPTL
Value::Value(const CppTL::ConstString& value)
	: type_(stringValue), allocated_(true)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.string_ = duplicateStringValue(value, value.length());
}
#endif

Value::Value(bool value)
	: type_(booleanValue), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(0), limit_(0) {
  value_.bool_ = value;
}

Value::Value(const Value& other)
	: type_(other.type_), allocated_(false)
#ifdef JSON_VALUE_USE_INTERNAL_MAP
	  ,
	  itemIsUsed_(0)
#endif
	  ,
	  comments_(0), start_(other.start_), limit_(other.limit_) {
  switch (type_) {
  case nullValue:
  case intValue:
  case uintValue:
  case realValue:
  case booleanValue:
	value_ = other.value_;
	break;
  case stringValue:
	if (other.value_.string_) {
	  value_.string_ = duplicateStringValue(other.value_.string_);
	  allocated_ = true;
	} else {
	  value_.string_ = 0;
	  allocated_ = false;
	}
	break;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
  case objectValue:
	value_.map_ = new ObjectValues(*other.value_.map_);
	break;
#else
  case arrayValue:
	value_.array_ = arrayAllocator()->newArrayCopy(*other.value_.array_);
	break;
  case objectValue:
	value_.map_ = mapAllocator()->newMapCopy(*other.value_.map_);
	break;
#endif
  default:
	JSON_ASSERT_UNREACHABLE;
  }
  if (other.comments_) {
	comments_ = new CommentInfo[numberOfCommentPlacement];
	for (int comment = 0; comment < numberOfCommentPlacement; ++comment) {
	  const CommentInfo& otherComment = other.comments_[comment];
	  if (otherComment.comment_)
		comments_[comment].setComment(otherComment.comment_);
	}
  }
}

Value::~Value() {
  switch (type_) {
  case nullValue:
  case intValue:
  case uintValue:
  case realValue:
  case booleanValue:
	break;
  case stringValue:
	if (allocated_)
	  releaseStringValue(value_.string_);
	break;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
  case objectValue:
	delete value_.map_;
	break;
#else
  case arrayValue:
	arrayAllocator()->destructArray(value_.array_);
	break;
  case objectValue:
	mapAllocator()->destructMap(value_.map_);
	break;
#endif
  default:
	JSON_ASSERT_UNREACHABLE;
  }

  if (comments_)
	delete[] comments_;
}

Value& Value::operator=(Value other) {
  swap(other);
  return *this;
}

void Value::swap(Value& other) {
  ValueType temp = type_;
  type_ = other.type_;
  other.type_ = temp;
  std::swap(value_, other.value_);
  int temp2 = allocated_;
  allocated_ = other.allocated_;
  other.allocated_ = temp2;
  std::swap(start_, other.start_);
  std::swap(limit_, other.limit_);
}

ValueType Value::type() const { return type_; }

int Value::compare(const Value& other) const {
  if (*this < other)
	return -1;
  if (*this > other)
	return 1;
  return 0;
}

bool Value::operator<(const Value& other) const {
  int typeDelta = type_ - other.type_;
  if (typeDelta)
	return typeDelta < 0 ? true : false;
  switch (type_) {
  case nullValue:
	return false;
  case intValue:
	return value_.int_ < other.value_.int_;
  case uintValue:
	return value_.uint_ < other.value_.uint_;
  case realValue:
	return value_.real_ < other.value_.real_;
  case booleanValue:
	return value_.bool_ < other.value_.bool_;
  case stringValue:
	return (value_.string_ == 0 && other.value_.string_) ||
		   (other.value_.string_ && value_.string_ &&
			strcmp(value_.string_, other.value_.string_) < 0);
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
  case objectValue: {
	int delta = int(value_.map_->size() - other.value_.map_->size());
	if (delta)
	  return delta < 0;
	return (*value_.map_) < (*other.value_.map_);
  }
#else
  case arrayValue:
	return value_.array_->compare(*(other.value_.array_)) < 0;
  case objectValue:
	return value_.map_->compare(*(other.value_.map_)) < 0;
#endif
  default:
	JSON_ASSERT_UNREACHABLE;
  }
  return false; // unreachable
}

bool Value::operator<=(const Value& other) const { return !(other < *this); }

bool Value::operator>=(const Value& other) const { return !(*this < other); }

bool Value::operator>(const Value& other) const { return other < *this; }

bool Value::operator==(const Value& other) const {
  // if ( type_ != other.type_ )
  // GCC 2.95.3 says:
  // attempt to take address of bit-field structure member `Json::Value::type_'
  // Beats me, but a temp solves the problem.
  int temp = other.type_;
  if (type_ != temp)
	return false;
  switch (type_) {
  case nullValue:
	return true;
  case intValue:
	return value_.int_ == other.value_.int_;
  case uintValue:
	return value_.uint_ == other.value_.uint_;
  case realValue:
	return value_.real_ == other.value_.real_;
  case booleanValue:
	return value_.bool_ == other.value_.bool_;
  case stringValue:
	return (value_.string_ == other.value_.string_) ||
		   (other.value_.string_ && value_.string_ &&
			strcmp(value_.string_, other.value_.string_) == 0);
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
  case objectValue:
	return value_.map_->size() == other.value_.map_->size() &&
		   (*value_.map_) == (*other.value_.map_);
#else
  case arrayValue:
	return value_.array_->compare(*(other.value_.array_)) == 0;
  case objectValue:
	return value_.map_->compare(*(other.value_.map_)) == 0;
#endif
  default:
	JSON_ASSERT_UNREACHABLE;
  }
  return false; // unreachable
}

bool Value::operator!=(const Value& other) const { return !(*this == other); }

const char* Value::asCString() const {
  JSON_ASSERT_MESSAGE(type_ == stringValue,
					  "in Json::Value::asCString(): requires stringValue");
  return value_.string_;
}

std::string Value::asString() const {
  switch (type_) {
  case nullValue:
	return "";
  case stringValue:
	return value_.string_ ? value_.string_ : "";
  case booleanValue:
	return value_.bool_ ? "true" : "false";
  case intValue:
	return valueToString(value_.int_);
  case uintValue:
	return valueToString(value_.uint_);
  case realValue:
	return valueToString(value_.real_);
  default:
	JSON_FAIL_MESSAGE("Type is not convertible to string");
  }
}

#ifdef JSON_USE_CPPTL
CppTL::ConstString Value::asConstString() const {
  return CppTL::ConstString(asString().c_str());
}
#endif

Value::Int Value::asInt() const {
  switch (type_) {
  case intValue:
	JSON_ASSERT_MESSAGE(isInt(), "LargestInt out of Int range");
	return Int(value_.int_);
  case uintValue:
	JSON_ASSERT_MESSAGE(isInt(), "LargestUInt out of Int range");
	return Int(value_.uint_);
  case realValue:
	JSON_ASSERT_MESSAGE(InRange(value_.real_, minInt, maxInt),
						"double out of Int range");
	return Int(value_.real_);
  case nullValue:
	return 0;
  case booleanValue:
	return value_.bool_ ? 1 : 0;
  default:
	break;
  }
  JSON_FAIL_MESSAGE("Value is not convertible to Int.");
}

Value::UInt Value::asUInt() const {
  switch (type_) {
  case intValue:
	JSON_ASSERT_MESSAGE(isUInt(), "LargestInt out of UInt range");
	return UInt(value_.int_);
  case uintValue:
	JSON_ASSERT_MESSAGE(isUInt(), "LargestUInt out of UInt range");
	return UInt(value_.uint_);
  case realValue:
	JSON_ASSERT_MESSAGE(InRange(value_.real_, 0, maxUInt),
						"double out of UInt range");
	return UInt(value_.real_);
  case nullValue:
	return 0;
  case booleanValue:
	return value_.bool_ ? 1 : 0;
  default:
	break;
  }
  JSON_FAIL_MESSAGE("Value is not convertible to UInt.");
}

#if defined(JSON_HAS_INT64)

Value::Int64 Value::asInt64() const {
  switch (type_) {
  case intValue:
	return Int64(value_.int_);
  case uintValue:
	JSON_ASSERT_MESSAGE(isInt64(), "LargestUInt out of Int64 range");
	return Int64(value_.uint_);
  case realValue:
	JSON_ASSERT_MESSAGE(InRange(value_.real_, minInt64, maxInt64),
						"double out of Int64 range");
	return Int64(value_.real_);
  case nullValue:
	return 0;
  case booleanValue:
	return value_.bool_ ? 1 : 0;
  default:
	break;
  }
  JSON_FAIL_MESSAGE("Value is not convertible to Int64.");
}

Value::UInt64 Value::asUInt64() const {
  switch (type_) {
  case intValue:
	JSON_ASSERT_MESSAGE(isUInt64(), "LargestInt out of UInt64 range");
	return UInt64(value_.int_);
  case uintValue:
	return UInt64(value_.uint_);
  case realValue:
	JSON_ASSERT_MESSAGE(InRange(value_.real_, 0, maxUInt64),
						"double out of UInt64 range");
	return UInt64(value_.real_);
  case nullValue:
	return 0;
  case booleanValue:
	return value_.bool_ ? 1 : 0;
  default:
	break;
  }
  JSON_FAIL_MESSAGE("Value is not convertible to UInt64.");
}
#endif // if defined(JSON_HAS_INT64)

LargestInt Value::asLargestInt() const {
#if defined(JSON_NO_INT64)
  return asInt();
#else
  return asInt64();
#endif
}

LargestUInt Value::asLargestUInt() const {
#if defined(JSON_NO_INT64)
  return asUInt();
#else
  return asUInt64();
#endif
}

double Value::asDouble() const {
  switch (type_) {
  case intValue:
	return static_cast<double>(value_.int_);
  case uintValue:
#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
	return static_cast<double>(value_.uint_);
#else  // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
	return integerToDouble(value_.uint_);
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
  case realValue:
	return value_.real_;
  case nullValue:
	return 0.0;
  case booleanValue:
	return value_.bool_ ? 1.0 : 0.0;
  default:
	break;
  }
  JSON_FAIL_MESSAGE("Value is not convertible to double.");
}

float Value::asFloat() const {
  switch (type_) {
  case intValue:
	return static_cast<float>(value_.int_);
  case uintValue:
#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
	return static_cast<float>(value_.uint_);
#else  // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
	return integerToDouble(value_.uint_);
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
  case realValue:
	return static_cast<float>(value_.real_);
  case nullValue:
	return 0.0;
  case booleanValue:
	return value_.bool_ ? 1.0f : 0.0f;
  default:
	break;
  }
  JSON_FAIL_MESSAGE("Value is not convertible to float.");
}

bool Value::asBool() const {
  switch (type_) {
  case booleanValue:
	return value_.bool_;
  case nullValue:
	return false;
  case intValue:
	return value_.int_ ? true : false;
  case uintValue:
	return value_.uint_ ? true : false;
  case realValue:
	return value_.real_ ? true : false;
  default:
	break;
  }
  JSON_FAIL_MESSAGE("Value is not convertible to bool.");
}

bool Value::isConvertibleTo(ValueType other) const {
  switch (other) {
  case nullValue:
	return (isNumeric() && asDouble() == 0.0) ||
		   (type_ == booleanValue && value_.bool_ == false) ||
		   (type_ == stringValue && asString() == "") ||
		   (type_ == arrayValue && value_.map_->size() == 0) ||
		   (type_ == objectValue && value_.map_->size() == 0) ||
		   type_ == nullValue;
  case intValue:
	return isInt() ||
		   (type_ == realValue && InRange(value_.real_, minInt, maxInt)) ||
		   type_ == booleanValue || type_ == nullValue;
  case uintValue:
	return isUInt() ||
		   (type_ == realValue && InRange(value_.real_, 0, maxUInt)) ||
		   type_ == booleanValue || type_ == nullValue;
  case realValue:
	return isNumeric() || type_ == booleanValue || type_ == nullValue;
  case booleanValue:
	return isNumeric() || type_ == booleanValue || type_ == nullValue;
  case stringValue:
	return isNumeric() || type_ == booleanValue || type_ == stringValue ||
		   type_ == nullValue;
  case arrayValue:
	return type_ == arrayValue || type_ == nullValue;
  case objectValue:
	return type_ == objectValue || type_ == nullValue;
  }
  JSON_ASSERT_UNREACHABLE;
  return false;
}

/// Number of values in array or object
ArrayIndex Value::size() const {
  switch (type_) {
  case nullValue:
  case intValue:
  case uintValue:
  case realValue:
  case booleanValue:
  case stringValue:
	return 0;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue: // size of the array is highest index + 1
	if (!value_.map_->empty()) {
	  ObjectValues::const_iterator itLast = value_.map_->end();
	  --itLast;
	  return (*itLast).first.index() + 1;
	}
	return 0;
  case objectValue:
	return ArrayIndex(value_.map_->size());
#else
  case arrayValue:
	return Int(value_.array_->size());
  case objectValue:
	return Int(value_.map_->size());
#endif
  }
  JSON_ASSERT_UNREACHABLE;
  return 0; // unreachable;
}

bool Value::empty() const {
  if (isNull() || isArray() || isObject())
	return size() == 0u;
  else
	return false;
}

bool Value::operator!() const { return isNull(); }

void Value::clear() {
  JSON_ASSERT_MESSAGE(type_ == nullValue || type_ == arrayValue ||
						  type_ == objectValue,
					  "in Json::Value::clear(): requires complex value");
  start_ = 0;
  limit_ = 0;
  switch (type_) {
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
  case objectValue:
	value_.map_->clear();
	break;
#else
  case arrayValue:
	value_.array_->clear();
	break;
  case objectValue:
	value_.map_->clear();
	break;
#endif
  default:
	break;
  }
}

void Value::resize(ArrayIndex newSize) {
  JSON_ASSERT_MESSAGE(type_ == nullValue || type_ == arrayValue,
					  "in Json::Value::resize(): requires arrayValue");
  if (type_ == nullValue)
	*this = Value(arrayValue);
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  ArrayIndex oldSize = size();
  if (newSize == 0)
	clear();
  else if (newSize > oldSize)
	(*this)[newSize - 1];
  else {
	for (ArrayIndex index = newSize; index < oldSize; ++index) {
	  value_.map_->erase(index);
	}
	assert(size() == newSize);
  }
#else
  value_.array_->resize(newSize);
#endif
}

Value& Value::operator[](ArrayIndex index) {
  JSON_ASSERT_MESSAGE(
	  type_ == nullValue || type_ == arrayValue,
	  "in Json::Value::operator[](ArrayIndex): requires arrayValue");
  if (type_ == nullValue)
	*this = Value(arrayValue);
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  CZString key(index);
  ObjectValues::iterator it = value_.map_->lower_bound(key);
  if (it != value_.map_->end() && (*it).first == key)
	return (*it).second;

  ObjectValues::value_type defaultValue(key, null);
  it = value_.map_->insert(it, defaultValue);
  return (*it).second;
#else
  return value_.array_->resolveReference(index);
#endif
}

Value& Value::operator[](int index) {
  JSON_ASSERT_MESSAGE(
	  index >= 0,
	  "in Json::Value::operator[](int index): index cannot be negative");
  return (*this)[ArrayIndex(index)];
}

const Value& Value::operator[](ArrayIndex index) const {
  JSON_ASSERT_MESSAGE(
	  type_ == nullValue || type_ == arrayValue,
	  "in Json::Value::operator[](ArrayIndex)const: requires arrayValue");
  if (type_ == nullValue)
	return null;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  CZString key(index);
  ObjectValues::const_iterator it = value_.map_->find(key);
  if (it == value_.map_->end())
	return null;
  return (*it).second;
#else
  Value* value = value_.array_->find(index);
  return value ? *value : null;
#endif
}

const Value& Value::operator[](int index) const {
  JSON_ASSERT_MESSAGE(
	  index >= 0,
	  "in Json::Value::operator[](int index) const: index cannot be negative");
  return (*this)[ArrayIndex(index)];
}

Value& Value::operator[](const char* key) {
  return resolveReference(key, false);
}

Value& Value::resolveReference(const char* key, bool isStatic) {
  JSON_ASSERT_MESSAGE(
	  type_ == nullValue || type_ == objectValue,
	  "in Json::Value::resolveReference(): requires objectValue");
  if (type_ == nullValue)
	*this = Value(objectValue);
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  CZString actualKey(
	  key, isStatic ? CZString::noDuplication : CZString::duplicateOnCopy);
  ObjectValues::iterator it = value_.map_->lower_bound(actualKey);
  if (it != value_.map_->end() && (*it).first == actualKey)
	return (*it).second;

  ObjectValues::value_type defaultValue(actualKey, null);
  it = value_.map_->insert(it, defaultValue);
  Value& value = (*it).second;
  return value;
#else
  return value_.map_->resolveReference(key, isStatic);
#endif
}

Value Value::get(ArrayIndex index, const Value& defaultValue) const {
  const Value* value = &((*this)[index]);
  return value == &null ? defaultValue : *value;
}

bool Value::isValidIndex(ArrayIndex index) const { return index < size(); }

const Value& Value::operator[](const char* key) const {
  JSON_ASSERT_MESSAGE(
	  type_ == nullValue || type_ == objectValue,
	  "in Json::Value::operator[](char const*)const: requires objectValue");
  if (type_ == nullValue)
	return null;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  CZString actualKey(key, CZString::noDuplication);
  ObjectValues::const_iterator it = value_.map_->find(actualKey);
  if (it == value_.map_->end())
	return null;
  return (*it).second;
#else
  const Value* value = value_.map_->find(key);
  return value ? *value : null;
#endif
}

Value& Value::operator[](const std::string& key) {
  return (*this)[key.c_str()];
}

const Value& Value::operator[](const std::string& key) const {
  return (*this)[key.c_str()];
}

Value& Value::operator[](const StaticString& key) {
  return resolveReference(key, true);
}

#ifdef JSON_USE_CPPTL
Value& Value::operator[](const CppTL::ConstString& key) {
  return (*this)[key.c_str()];
}

const Value& Value::operator[](const CppTL::ConstString& key) const {
  return (*this)[key.c_str()];
}
#endif

Value& Value::append(const Value& value) { return (*this)[size()] = value; }

Value Value::get(const char* key, const Value& defaultValue) const {
  const Value* value = &((*this)[key]);
  return value == &null ? defaultValue : *value;
}

Value Value::get(const std::string& key, const Value& defaultValue) const {
  return get(key.c_str(), defaultValue);
}

Value Value::removeMember(const char* key) {
  JSON_ASSERT_MESSAGE(type_ == nullValue || type_ == objectValue,
					  "in Json::Value::removeMember(): requires objectValue");
  if (type_ == nullValue)
	return null;
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  CZString actualKey(key, CZString::noDuplication);
  ObjectValues::iterator it = value_.map_->find(actualKey);
  if (it == value_.map_->end())
	return null;
  Value old(it->second);
  value_.map_->erase(it);
  return old;
#else
  Value* value = value_.map_->find(key);
  if (value) {
	Value old(*value);
	value_.map_.remove(key);
	return old;
  } else {
	return null;
  }
#endif
}

Value Value::removeMember(const std::string& key) {
  return removeMember(key.c_str());
}

#ifdef JSON_USE_CPPTL
Value Value::get(const CppTL::ConstString& key,
				 const Value& defaultValue) const {
  return get(key.c_str(), defaultValue);
}
#endif

bool Value::isMember(const char* key) const {
  const Value* value = &((*this)[key]);
  return value != &null;
}

bool Value::isMember(const std::string& key) const {
  return isMember(key.c_str());
}

#ifdef JSON_USE_CPPTL
bool Value::isMember(const CppTL::ConstString& key) const {
  return isMember(key.c_str());
}
#endif

Value::Members Value::getMemberNames() const {
  JSON_ASSERT_MESSAGE(
	  type_ == nullValue || type_ == objectValue,
	  "in Json::Value::getMemberNames(), value must be objectValue");
  if (type_ == nullValue)
	return Value::Members();
  Members members;
  members.reserve(value_.map_->size());
#ifndef JSON_VALUE_USE_INTERNAL_MAP
  ObjectValues::const_iterator it = value_.map_->begin();
  ObjectValues::const_iterator itEnd = value_.map_->end();
  for (; it != itEnd; ++it)
	members.push_back(std::string((*it).first.c_str()));
#else
  ValueInternalMap::IteratorState it;
  ValueInternalMap::IteratorState itEnd;
  value_.map_->makeBeginIterator(it);
  value_.map_->makeEndIterator(itEnd);
  for (; !ValueInternalMap::equals(it, itEnd); ValueInternalMap::increment(it))
	members.push_back(std::string(ValueInternalMap::key(it)));
#endif
  return members;
}
//
//# ifdef JSON_USE_CPPTL
// EnumMemberNames
// Value::enumMemberNames() const
//{
//   if ( type_ == objectValue )
//   {
//      return CppTL::Enum::any(  CppTL::Enum::transform(
//         CppTL::Enum::keys( *(value_.map_), CppTL::Type<const CZString &>() ),
//         MemberNamesTransform() ) );
//   }
//   return EnumMemberNames();
//}
//
//
// EnumValues
// Value::enumValues() const
//{
//   if ( type_ == objectValue  ||  type_ == arrayValue )
//      return CppTL::Enum::anyValues( *(value_.map_),
//                                     CppTL::Type<const Value &>() );
//   return EnumValues();
//}
//
//# endif

static bool IsIntegral(double d) {
  double integral_part;
  return modf(d, &integral_part) == 0.0;
}

bool Value::isNull() const { return type_ == nullValue; }

bool Value::isBool() const { return type_ == booleanValue; }

bool Value::isInt() const {
  switch (type_) {
  case intValue:
	return value_.int_ >= minInt && value_.int_ <= maxInt;
  case uintValue:
	return value_.uint_ <= UInt(maxInt);
  case realValue:
	return value_.real_ >= minInt && value_.real_ <= maxInt &&
		   IsIntegral(value_.real_);
  default:
	break;
  }
  return false;
}

bool Value::isUInt() const {
  switch (type_) {
  case intValue:
	return value_.int_ >= 0 && LargestUInt(value_.int_) <= LargestUInt(maxUInt);
  case uintValue:
	return value_.uint_ <= maxUInt;
  case realValue:
	return value_.real_ >= 0 && value_.real_ <= maxUInt &&
		   IsIntegral(value_.real_);
  default:
	break;
  }
  return false;
}

bool Value::isInt64() const {
#if defined(JSON_HAS_INT64)
  switch (type_) {
  case intValue:
	return true;
  case uintValue:
	return value_.uint_ <= UInt64(maxInt64);
  case realValue:
	// Note that maxInt64 (= 2^63 - 1) is not exactly representable as a
	// double, so double(maxInt64) will be rounded up to 2^63. Therefore we
	// require the value to be strictly less than the limit.
	return value_.real_ >= double(minInt64) &&
		   value_.real_ < double(maxInt64) && IsIntegral(value_.real_);
  default:
	break;
  }
#endif // JSON_HAS_INT64
  return false;
}

bool Value::isUInt64() const {
#if defined(JSON_HAS_INT64)
  switch (type_) {
  case intValue:
	return value_.int_ >= 0;
  case uintValue:
	return true;
  case realValue:
	// Note that maxUInt64 (= 2^64 - 1) is not exactly representable as a
	// double, so double(maxUInt64) will be rounded up to 2^64. Therefore we
	// require the value to be strictly less than the limit.
	return value_.real_ >= 0 && value_.real_ < maxUInt64AsDouble &&
		   IsIntegral(value_.real_);
  default:
	break;
  }
#endif // JSON_HAS_INT64
  return false;
}

bool Value::isIntegral() const {
#if defined(JSON_HAS_INT64)
  return isInt64() || isUInt64();
#else
  return isInt() || isUInt();
#endif
}

bool Value::isDouble() const { return type_ == realValue || isIntegral(); }

bool Value::isNumeric() const { return isIntegral() || isDouble(); }

bool Value::isString() const { return type_ == stringValue; }

bool Value::isArray() const { return type_ == arrayValue; }

bool Value::isObject() const { return type_ == objectValue; }

void Value::setComment(const char* comment, CommentPlacement placement) {
  if (!comments_)
	comments_ = new CommentInfo[numberOfCommentPlacement];
  comments_[placement].setComment(comment);
}

void Value::setComment(const std::string& comment, CommentPlacement placement) {
  setComment(comment.c_str(), placement);
}

bool Value::hasComment(CommentPlacement placement) const {
  return comments_ != 0 && comments_[placement].comment_ != 0;
}

std::string Value::getComment(CommentPlacement placement) const {
  if (hasComment(placement))
	return comments_[placement].comment_;
  return "";
}

void Value::setOffsetStart(size_t start) { start_ = start; }

void Value::setOffsetLimit(size_t limit) { limit_ = limit; }

size_t Value::getOffsetStart() const { return start_; }

size_t Value::getOffsetLimit() const { return limit_; }

std::string Value::toStyledString() const {
  StyledWriter writer;
  return writer.write(*this);
}

Value::const_iterator Value::begin() const {
  switch (type_) {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
	if (value_.array_) {
	  ValueInternalArray::IteratorState it;
	  value_.array_->makeBeginIterator(it);
	  return const_iterator(it);
	}
	break;
  case objectValue:
	if (value_.map_) {
	  ValueInternalMap::IteratorState it;
	  value_.map_->makeBeginIterator(it);
	  return const_iterator(it);
	}
	break;
#else
  case arrayValue:
  case objectValue:
	if (value_.map_)
	  return const_iterator(value_.map_->begin());
	break;
#endif
  default:
	break;
  }
  return const_iterator();
}

Value::const_iterator Value::end() const {
  switch (type_) {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
	if (value_.array_) {
	  ValueInternalArray::IteratorState it;
	  value_.array_->makeEndIterator(it);
	  return const_iterator(it);
	}
	break;
  case objectValue:
	if (value_.map_) {
	  ValueInternalMap::IteratorState it;
	  value_.map_->makeEndIterator(it);
	  return const_iterator(it);
	}
	break;
#else
  case arrayValue:
  case objectValue:
	if (value_.map_)
	  return const_iterator(value_.map_->end());
	break;
#endif
  default:
	break;
  }
  return const_iterator();
}

Value::iterator Value::begin() {
  switch (type_) {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
	if (value_.array_) {
	  ValueInternalArray::IteratorState it;
	  value_.array_->makeBeginIterator(it);
	  return iterator(it);
	}
	break;
  case objectValue:
	if (value_.map_) {
	  ValueInternalMap::IteratorState it;
	  value_.map_->makeBeginIterator(it);
	  return iterator(it);
	}
	break;
#else
  case arrayValue:
  case objectValue:
	if (value_.map_)
	  return iterator(value_.map_->begin());
	break;
#endif
  default:
	break;
  }
  return iterator();
}

Value::iterator Value::end() {
  switch (type_) {
#ifdef JSON_VALUE_USE_INTERNAL_MAP
  case arrayValue:
	if (value_.array_) {
	  ValueInternalArray::IteratorState it;
	  value_.array_->makeEndIterator(it);
	  return iterator(it);
	}
	break;
  case objectValue:
	if (value_.map_) {
	  ValueInternalMap::IteratorState it;
	  value_.map_->makeEndIterator(it);
	  return iterator(it);
	}
	break;
#else
  case arrayValue:
  case objectValue:
	if (value_.map_)
	  return iterator(value_.map_->end());
	break;
#endif
  default:
	break;
  }
  return iterator();
}

// class PathArgument
// //////////////////////////////////////////////////////////////////

PathArgument::PathArgument() : key_(), index_(), kind_(kindNone) {}

PathArgument::PathArgument(ArrayIndex index)
	: key_(), index_(index), kind_(kindIndex) {}

PathArgument::PathArgument(const char* key)
	: key_(key), index_(), kind_(kindKey) {}

PathArgument::PathArgument(const std::string& key)
	: key_(key.c_str()), index_(), kind_(kindKey) {}

// class Path
// //////////////////////////////////////////////////////////////////

Path::Path(const std::string& path,
		   const PathArgument& a1,
		   const PathArgument& a2,
		   const PathArgument& a3,
		   const PathArgument& a4,
		   const PathArgument& a5) {
  InArgs in;
  in.push_back(&a1);
  in.push_back(&a2);
  in.push_back(&a3);
  in.push_back(&a4);
  in.push_back(&a5);
  makePath(path, in);
}

void Path::makePath(const std::string& path, const InArgs& in) {
  const char* current = path.c_str();
  const char* end = current + path.length();
  InArgs::const_iterator itInArg = in.begin();
  while (current != end) {
	if (*current == '[') {
	  ++current;
	  if (*current == '%')
		addPathInArg(path, in, itInArg, PathArgument::kindIndex);
	  else {
		ArrayIndex index = 0;
		for (; current != end && *current >= '0' && *current <= '9'; ++current)
		  index = index * 10 + ArrayIndex(*current - '0');
		args_.push_back(index);
	  }
	  if (current == end || *current++ != ']')
		invalidPath(path, int(current - path.c_str()));
	} else if (*current == '%') {
	  addPathInArg(path, in, itInArg, PathArgument::kindKey);
	  ++current;
	} else if (*current == '.') {
	  ++current;
	} else {
	  const char* beginName = current;
	  while (current != end && !strchr("[.", *current))
		++current;
	  args_.push_back(std::string(beginName, current));
	}
  }
}

void Path::addPathInArg(const std::string& /*path*/,
						const InArgs& in,
						InArgs::const_iterator& itInArg,
						PathArgument::Kind kind) {
  if (itInArg == in.end()) {
	// Error: missing argument %d
  } else if ((*itInArg)->kind_ != kind) {
	// Error: bad argument type
  } else {
	args_.push_back(**itInArg);
  }
}

void Path::invalidPath(const std::string& /*path*/, int /*location*/) {
  // Error: invalid path.
}

const Value& Path::resolve(const Value& root) const {
  const Value* node = &root;
  for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it) {
	const PathArgument& arg = *it;
	if (arg.kind_ == PathArgument::kindIndex) {
	  if (!node->isArray() || !node->isValidIndex(arg.index_)) {
		// Error: unable to resolve path (array value expected at position...
	  }
	  node = &((*node)[arg.index_]);
	} else if (arg.kind_ == PathArgument::kindKey) {
	  if (!node->isObject()) {
		// Error: unable to resolve path (object value expected at position...)
	  }
	  node = &((*node)[arg.key_]);
	  if (node == &Value::null) {
		// Error: unable to resolve path (object has no member named '' at
		// position...)
	  }
	}
  }
  return *node;
}

Value Path::resolve(const Value& root, const Value& defaultValue) const {
  const Value* node = &root;
  for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it) {
	const PathArgument& arg = *it;
	if (arg.kind_ == PathArgument::kindIndex) {
	  if (!node->isArray() || !node->isValidIndex(arg.index_))
		return defaultValue;
	  node = &((*node)[arg.index_]);
	} else if (arg.kind_ == PathArgument::kindKey) {
	  if (!node->isObject())
		return defaultValue;
	  node = &((*node)[arg.key_]);
	  if (node == &Value::null)
		return defaultValue;
	}
  }
  return *node;
}

Value& Path::make(Value& root) const {
  Value* node = &root;
  for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it) {
	const PathArgument& arg = *it;
	if (arg.kind_ == PathArgument::kindIndex) {
	  if (!node->isArray()) {
		// Error: node is not an array at position ...
	  }
	  node = &((*node)[arg.index_]);
	} else if (arg.kind_ == PathArgument::kindKey) {
	  if (!node->isObject()) {
		// Error: node is not an object at position...
	  }
	  node = &((*node)[arg.key_]);
	}
  }
  return *node;
}

} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_value.cpp
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: src/lib_json/json_writer.cpp
// //////////////////////////////////////////////////////////////////////

// Copyright 2011 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#if !defined(JSON_IS_AMALGAMATION)
#include <json/writer.h>
#include "json_tool.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <utility>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <math.h>

#if defined(_MSC_VER) && _MSC_VER < 1500 // VC++ 8.0 and below
#include <float.h>
#define isfinite _finite
#define snprintf _snprintf
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC++ 8.0
// Disable warning about strdup being deprecated.
#pragma warning(disable : 4996)
#endif

namespace Json {

static bool containsControlCharacter(const char* str) {
  while (*str) {
	if (isControlCharacter(*(str++)))
	  return true;
  }
  return false;
}

std::string valueToString(LargestInt value) {
  UIntToStringBuffer buffer;
  char* current = buffer + sizeof(buffer);
  bool isNegative = value < 0;
  if (isNegative)
	value = -value;
  uintToString(LargestUInt(value), current);
  if (isNegative)
	*--current = '-';
  assert(current >= buffer);
  return current;
}

std::string valueToString(LargestUInt value) {
  UIntToStringBuffer buffer;
  char* current = buffer + sizeof(buffer);
  uintToString(value, current);
  assert(current >= buffer);
  return current;
}

#if defined(JSON_HAS_INT64)

std::string valueToString(Int value) {
  return valueToString(LargestInt(value));
}

std::string valueToString(UInt value) {
  return valueToString(LargestUInt(value));
}

#endif // # if defined(JSON_HAS_INT64)

std::string valueToString(double value) {
  // Allocate a buffer that is more than large enough to store the 16 digits of
  // precision requested below.
  char buffer[32];
  int len = -1;

// Print into the buffer. We need not request the alternative representation
// that always has a decimal point because JSON doesn't distingish the
// concepts of reals and integers.
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__) // Use secure version with
													  // visual studio 2005 to
													  // avoid warning.
#if defined(WINCE)
  len = _snprintf(buffer, sizeof(buffer), "%.16g", value);
#else
  len = sprintf_s(buffer, sizeof(buffer), "%.16g", value);
#endif
#else
  if (std::isfinite(value)) {
	len = snprintf(buffer, sizeof(buffer), "%.16g", value);
  } else {
	// IEEE standard states that NaN values will not compare to themselves
	if (value != value) {
	  len = snprintf(buffer, sizeof(buffer), "null");
	} else if (value < 0) {
	  len = snprintf(buffer, sizeof(buffer), "-1e+9999");
	} else {
	  len = snprintf(buffer, sizeof(buffer), "1e+9999");
	}
	// For those, we do not need to call fixNumLoc, but it is fast.
  }
#endif
  assert(len >= 0);
  fixNumericLocale(buffer, buffer + len);
  return buffer;
}

std::string valueToString(bool value) { return value ? "true" : "false"; }

std::string valueToQuotedString(const char* value) {
  if (value == NULL)
	return "";
  // Not sure how to handle unicode...
  if (strpbrk(value, "\"\\\b\f\n\r\t") == NULL &&
	  !containsControlCharacter(value))
	return std::string("\"") + value + "\"";
  // We have to walk value and escape any special characters.
  // Appending to std::string is not efficient, but this should be rare.
  // (Note: forward slashes are *not* rare, but I am not escaping them.)
  std::string::size_type maxsize =
	  strlen(value) * 2 + 3; // allescaped+quotes+NULL
  std::string result;
  result.reserve(maxsize); // to avoid lots of mallocs
  result += "\"";
  for (const char* c = value; *c != 0; ++c) {
	switch (*c) {
	case '\"':
	  result += "\\\"";
	  break;
	case '\\':
	  result += "\\\\";
	  break;
	case '\b':
	  result += "\\b";
	  break;
	case '\f':
	  result += "\\f";
	  break;
	case '\n':
	  result += "\\n";
	  break;
	case '\r':
	  result += "\\r";
	  break;
	case '\t':
	  result += "\\t";
	  break;
	// case '/':
	// Even though \/ is considered a legal escape in JSON, a bare
	// slash is also legal, so I see no reason to escape it.
	// (I hope I am not misunderstanding something.
	// blep notes: actually escaping \/ may be useful in javascript to avoid </
	// sequence.
	// Should add a flag to allow this compatibility mode and prevent this
	// sequence from occurring.
	default:
	  if (isControlCharacter(*c)) {
		std::ostringstream oss;
		oss << "\\u" << std::hex << std::uppercase << std::setfill('0')
			<< std::setw(4) << static_cast<int>(*c);
		result += oss.str();
	  } else {
		result += *c;
	  }
	  break;
	}
  }
  result += "\"";
  return result;
}

// Class Writer
// //////////////////////////////////////////////////////////////////
Writer::~Writer() {}

// Class FastWriter
// //////////////////////////////////////////////////////////////////

FastWriter::FastWriter()
	: yamlCompatiblityEnabled_(false), dropNullPlaceholders_(false),
	  omitEndingLineFeed_(false) {}

void FastWriter::enableYAMLCompatibility() { yamlCompatiblityEnabled_ = true; }

void FastWriter::dropNullPlaceholders() { dropNullPlaceholders_ = true; }

void FastWriter::omitEndingLineFeed() { omitEndingLineFeed_ = true; }

std::string FastWriter::write(const Value& root) {
  document_ = "";
  writeValue(root);
  if (!omitEndingLineFeed_)
	document_ += "\n";
  return document_;
}

void FastWriter::writeValue(const Value& value) {
  switch (value.type()) {
  case nullValue:
	if (!dropNullPlaceholders_)
	  document_ += "null";
	break;
  case intValue:
	document_ += valueToString(value.asLargestInt());
	break;
  case uintValue:
	document_ += valueToString(value.asLargestUInt());
	break;
  case realValue:
	document_ += valueToString(value.asDouble());
	break;
  case stringValue:
	document_ += valueToQuotedString(value.asCString());
	break;
  case booleanValue:
	document_ += valueToString(value.asBool());
	break;
  case arrayValue: {
	document_ += "[";
	int size = value.size();
	for (int index = 0; index < size; ++index) {
	  if (index > 0)
		document_ += ",";
	  writeValue(value[index]);
	}
	document_ += "]";
  } break;
  case objectValue: {
	Value::Members members(value.getMemberNames());
	document_ += "{";
	for (Value::Members::iterator it = members.begin(); it != members.end();
		 ++it) {
	  const std::string& name = *it;
	  if (it != members.begin())
		document_ += ",";
	  document_ += valueToQuotedString(name.c_str());
	  document_ += yamlCompatiblityEnabled_ ? ": " : ":";
	  writeValue(value[name]);
	}
	document_ += "}";
  } break;
  }
}

// Class StyledWriter
// //////////////////////////////////////////////////////////////////

StyledWriter::StyledWriter()
	: rightMargin_(74), indentSize_(3), addChildValues_() {}

std::string StyledWriter::write(const Value& root) {
  document_ = "";
  addChildValues_ = false;
  indentString_ = "";
  writeCommentBeforeValue(root);
  writeValue(root);
  writeCommentAfterValueOnSameLine(root);
  document_ += "\n";
  return document_;
}

void StyledWriter::writeValue(const Value& value) {
  switch (value.type()) {
  case nullValue:
	pushValue("null");
	break;
  case intValue:
	pushValue(valueToString(value.asLargestInt()));
	break;
  case uintValue:
	pushValue(valueToString(value.asLargestUInt()));
	break;
  case realValue:
	pushValue(valueToString(value.asDouble()));
	break;
  case stringValue:
	pushValue(valueToQuotedString(value.asCString()));
	break;
  case booleanValue:
	pushValue(valueToString(value.asBool()));
	break;
  case arrayValue:
	writeArrayValue(value);
	break;
  case objectValue: {
	Value::Members members(value.getMemberNames());
	if (members.empty())
	  pushValue("{}");
	else {
	  writeWithIndent("{");
	  indent();
	  Value::Members::iterator it = members.begin();
	  for (;;) {
		const std::string& name = *it;
		const Value& childValue = value[name];
		writeCommentBeforeValue(childValue);
		writeWithIndent(valueToQuotedString(name.c_str()));
		document_ += " : ";
		writeValue(childValue);
		if (++it == members.end()) {
		  writeCommentAfterValueOnSameLine(childValue);
		  break;
		}
		document_ += ",";
		writeCommentAfterValueOnSameLine(childValue);
	  }
	  unindent();
	  writeWithIndent("}");
	}
  } break;
  }
}

void StyledWriter::writeArrayValue(const Value& value) {
  unsigned size = value.size();
  if (size == 0)
	pushValue("[]");
  else {
	bool isArrayMultiLine = isMultineArray(value);
	if (isArrayMultiLine) {
	  writeWithIndent("[");
	  indent();
	  bool hasChildValue = !childValues_.empty();
	  unsigned index = 0;
	  for (;;) {
		const Value& childValue = value[index];
		writeCommentBeforeValue(childValue);
		if (hasChildValue)
		  writeWithIndent(childValues_[index]);
		else {
		  writeIndent();
		  writeValue(childValue);
		}
		if (++index == size) {
		  writeCommentAfterValueOnSameLine(childValue);
		  break;
		}
		document_ += ",";
		writeCommentAfterValueOnSameLine(childValue);
	  }
	  unindent();
	  writeWithIndent("]");
	} else // output on a single line
	{
	  assert(childValues_.size() == size);
	  document_ += "[ ";
	  for (unsigned index = 0; index < size; ++index) {
		if (index > 0)
		  document_ += ", ";
		document_ += childValues_[index];
	  }
	  document_ += " ]";
	}
  }
}

bool StyledWriter::isMultineArray(const Value& value) {
  int size = value.size();
  bool isMultiLine = size * 3 >= rightMargin_;
  childValues_.clear();
  for (int index = 0; index < size && !isMultiLine; ++index) {
	const Value& childValue = value[index];
	isMultiLine =
		isMultiLine || ((childValue.isArray() || childValue.isObject()) &&
						childValue.size() > 0);
  }
  if (!isMultiLine) // check if line length > max line length
  {
	childValues_.reserve(size);
	addChildValues_ = true;
	int lineLength = 4 + (size - 1) * 2; // '[ ' + ', '*n + ' ]'
	for (int index = 0; index < size; ++index) {
	  writeValue(value[index]);
	  lineLength += int(childValues_[index].length());
	}
	addChildValues_ = false;
	isMultiLine = isMultiLine || lineLength >= rightMargin_;
  }
  return isMultiLine;
}

void StyledWriter::pushValue(const std::string& value) {
  if (addChildValues_)
	childValues_.push_back(value);
  else
	document_ += value;
}

void StyledWriter::writeIndent() {
  if (!document_.empty()) {
	char last = document_[document_.length() - 1];
	if (last == ' ') // already indented
	  return;
	if (last != '\n') // Comments may add new-line
	  document_ += '\n';
  }
  document_ += indentString_;
}

void StyledWriter::writeWithIndent(const std::string& value) {
  writeIndent();
  document_ += value;
}

void StyledWriter::indent() { indentString_ += std::string(indentSize_, ' '); }

void StyledWriter::unindent() {
  assert(int(indentString_.size()) >= indentSize_);
  indentString_.resize(indentString_.size() - indentSize_);
}

void StyledWriter::writeCommentBeforeValue(const Value& root) {
  if (!root.hasComment(commentBefore))
	return;

  document_ += "\n";
  writeIndent();
  std::string normalizedComment = normalizeEOL(root.getComment(commentBefore));
  std::string::const_iterator iter = normalizedComment.begin();
  while (iter != normalizedComment.end()) {
	document_ += *iter;
	if (*iter == '\n' && *(iter + 1) == '/')
	  writeIndent();
	++iter;
  }

  // Comments are stripped of newlines, so add one here
  document_ += "\n";
}

void StyledWriter::writeCommentAfterValueOnSameLine(const Value& root) {
  if (root.hasComment(commentAfterOnSameLine))
	document_ += " " + normalizeEOL(root.getComment(commentAfterOnSameLine));

  if (root.hasComment(commentAfter)) {
	document_ += "\n";
	document_ += normalizeEOL(root.getComment(commentAfter));
	document_ += "\n";
  }
}

bool StyledWriter::hasCommentForValue(const Value& value) {
  return value.hasComment(commentBefore) ||
		 value.hasComment(commentAfterOnSameLine) ||
		 value.hasComment(commentAfter);
}

std::string StyledWriter::normalizeEOL(const std::string& text) {
  std::string normalized;
  normalized.reserve(text.length());
  const char* begin = text.c_str();
  const char* end = begin + text.length();
  const char* current = begin;
  while (current != end) {
	char c = *current++;
	if (c == '\r') // mac or dos EOL
	{
	  if (*current == '\n') // convert dos EOL
		++current;
	  normalized += '\n';
	} else // handle unix EOL & other char
	  normalized += c;
  }
  return normalized;
}

// Class StyledStreamWriter
// //////////////////////////////////////////////////////////////////

StyledStreamWriter::StyledStreamWriter(std::string indentation)
	: document_(NULL), rightMargin_(74), indentation_(indentation),
	  addChildValues_() {}

void StyledStreamWriter::write(std::ostream& out, const Value& root) {
  document_ = &out;
  addChildValues_ = false;
  indentString_ = "";
  writeCommentBeforeValue(root);
  writeValue(root);
  writeCommentAfterValueOnSameLine(root);
  *document_ << "\n";
  document_ = NULL; // Forget the stream, for safety.
}

void StyledStreamWriter::writeValue(const Value& value) {
  switch (value.type()) {
  case nullValue:
	pushValue("null");
	break;
  case intValue:
	pushValue(valueToString(value.asLargestInt()));
	break;
  case uintValue:
	pushValue(valueToString(value.asLargestUInt()));
	break;
  case realValue:
	pushValue(valueToString(value.asDouble()));
	break;
  case stringValue:
	pushValue(valueToQuotedString(value.asCString()));
	break;
  case booleanValue:
	pushValue(valueToString(value.asBool()));
	break;
  case arrayValue:
	writeArrayValue(value);
	break;
  case objectValue: {
	Value::Members members(value.getMemberNames());
	if (members.empty())
	  pushValue("{}");
	else {
	  writeWithIndent("{");
	  indent();
	  Value::Members::iterator it = members.begin();
	  for (;;) {
		const std::string& name = *it;
		const Value& childValue = value[name];
		writeCommentBeforeValue(childValue);
		writeWithIndent(valueToQuotedString(name.c_str()));
		*document_ << " : ";
		writeValue(childValue);
		if (++it == members.end()) {
		  writeCommentAfterValueOnSameLine(childValue);
		  break;
		}
		*document_ << ",";
		writeCommentAfterValueOnSameLine(childValue);
	  }
	  unindent();
	  writeWithIndent("}");
	}
  } break;
  }
}

void StyledStreamWriter::writeArrayValue(const Value& value) {
  unsigned size = value.size();
  if (size == 0)
	pushValue("[]");
  else {
	bool isArrayMultiLine = isMultineArray(value);
	if (isArrayMultiLine) {
	  writeWithIndent("[");
	  indent();
	  bool hasChildValue = !childValues_.empty();
	  unsigned index = 0;
	  for (;;) {
		const Value& childValue = value[index];
		writeCommentBeforeValue(childValue);
		if (hasChildValue)
		  writeWithIndent(childValues_[index]);
		else {
		  writeIndent();
		  writeValue(childValue);
		}
		if (++index == size) {
		  writeCommentAfterValueOnSameLine(childValue);
		  break;
		}
		*document_ << ",";
		writeCommentAfterValueOnSameLine(childValue);
	  }
	  unindent();
	  writeWithIndent("]");
	} else // output on a single line
	{
	  assert(childValues_.size() == size);
	  *document_ << "[ ";
	  for (unsigned index = 0; index < size; ++index) {
		if (index > 0)
		  *document_ << ", ";
		*document_ << childValues_[index];
	  }
	  *document_ << " ]";
	}
  }
}

bool StyledStreamWriter::isMultineArray(const Value& value) {
  int size = value.size();
  bool isMultiLine = size * 3 >= rightMargin_;
  childValues_.clear();
  for (int index = 0; index < size && !isMultiLine; ++index) {
	const Value& childValue = value[index];
	isMultiLine =
		isMultiLine || ((childValue.isArray() || childValue.isObject()) &&
						childValue.size() > 0);
  }
  if (!isMultiLine) // check if line length > max line length
  {
	childValues_.reserve(size);
	addChildValues_ = true;
	int lineLength = 4 + (size - 1) * 2; // '[ ' + ', '*n + ' ]'
	for (int index = 0; index < size; ++index) {
	  writeValue(value[index]);
	  lineLength += int(childValues_[index].length());
	}
	addChildValues_ = false;
	isMultiLine = isMultiLine || lineLength >= rightMargin_;
  }
  return isMultiLine;
}

void StyledStreamWriter::pushValue(const std::string& value) {
  if (addChildValues_)
	childValues_.push_back(value);
  else
	*document_ << value;
}

void StyledStreamWriter::writeIndent() {
  /*
	Some comments in this method would have been nice. ;-)

   if ( !document_.empty() )
   {
	  char last = document_[document_.length()-1];
	  if ( last == ' ' )     // already indented
		 return;
	  if ( last != '\n' )    // Comments may add new-line
		 *document_ << '\n';
   }
  */
  *document_ << '\n' << indentString_;
}

void StyledStreamWriter::writeWithIndent(const std::string& value) {
  writeIndent();
  *document_ << value;
}

void StyledStreamWriter::indent() { indentString_ += indentation_; }

void StyledStreamWriter::unindent() {
  assert(indentString_.size() >= indentation_.size());
  indentString_.resize(indentString_.size() - indentation_.size());
}

void StyledStreamWriter::writeCommentBeforeValue(const Value& root) {
  if (!root.hasComment(commentBefore))
	return;
  *document_ << normalizeEOL(root.getComment(commentBefore));
  *document_ << "\n";
}

void StyledStreamWriter::writeCommentAfterValueOnSameLine(const Value& root) {
  if (root.hasComment(commentAfterOnSameLine))
	*document_ << " " + normalizeEOL(root.getComment(commentAfterOnSameLine));

  if (root.hasComment(commentAfter)) {
	*document_ << "\n";
	*document_ << normalizeEOL(root.getComment(commentAfter));
	*document_ << "\n";
  }
}

bool StyledStreamWriter::hasCommentForValue(const Value& value) {
  return value.hasComment(commentBefore) ||
		 value.hasComment(commentAfterOnSameLine) ||
		 value.hasComment(commentAfter);
}

std::string StyledStreamWriter::normalizeEOL(const std::string& text) {
  std::string normalized;
  normalized.reserve(text.length());
  const char* begin = text.c_str();
  const char* end = begin + text.length();
  const char* current = begin;
  while (current != end) {
	char c = *current++;
	if (c == '\r') // mac or dos EOL
	{
	  if (*current == '\n') // convert dos EOL
		++current;
	  normalized += '\n';
	} else // handle unix EOL & other char
	  normalized += c;
  }
  return normalized;
}

std::ostream& operator<<(std::ostream& sout, const Value& root) {
  Json::StyledStreamWriter writer;
  writer.write(sout, root);
  return sout;
}

} // namespace Json

// //////////////////////////////////////////////////////////////////////
// End of content of file: src/lib_json/json_writer.cpp
// //////////////////////////////////////////////////////////////////////

/*** End of inlined file: jsoncpp.cpp ***/


#endif
/*** End of inlined file: json.h ***/


#else

#endif

#define SIMPLEIO 0
//由玩家自己定义，0表示JSON交互，1表示简单交互。

using namespace std;

vector<string> request, response;
vector<string> hand,unhand;//当前的手牌 暗杠的手牌
vector<pair<string, pair<string, int> > > pack;

typedef pair<int,int> pii;
pii f(string stmp)//牌型转化
{
	pii a;
	switch(stmp[0]){
		case 'W':a.first=1;break;
		case 'B':a.first=2;break;
		case 'T':a.first=3;break;
		case 'F':a.first=4;break;
		case 'J':a.first=5;break;
	}
	a.second=stmp[1]-'0';
	return a;
}
string ff(int x,int y)//牌型转化2
{
   string s="";
   switch(x){
		case 1:s+='W';break;
		case 2:s+='B';break;
		case 3:s+='T';break;
		case 4:s+='F';break;
		case 5:s+='J';break;
	}
	s+=char('0'+y);
	return  s;
}
int fff(int myID,int playID)//判断是那家供牌
{
	if((myID-1+4)%4==playID)return 0;//上家供牌
	if((myID+1)%4==playID)return 2;//下家供牌
	return 1;//对家供牌
}
bool CheckHu(int num[10],int k){
	for(int i=1;i<=k;i++)if(num[i]<0)return false;
	if(num[0]==0)return true;
	if(num[0]==1)return false;
	if(num[0]==2){
		for(int i=1;i<=k;i++)if(num[i]==2)return true;
		return false;
	}
	if(k==9){
		for(int i=1;i<=9;i++){
			if(num[i]>0){
				if(i<=7){
					num[i]--,num[i+1]--,num[i+2]--;
					num[0]=num[0]-3;
					if(CheckHu(num,k))return true;
					num[0]=num[0]+3;
					num[i]++,num[i+1]++,num[i+2]++;
				}
				if(num[i]>=3){
					num[i]-=3;num[0]-=3;
					if(CheckHu(num,k))return true;
					num[i]+=3;num[0]+=3;
				}
			}
		}return false;
	}
	else
	{
		for(int i=1;i<=k;i++){
			if(num[i]>0){
				if(num[i]>=3){
					num[i]-=3;num[0]-=3;
					if(CheckHu(num,k))return true;
					num[i]+=3;num[0]+=3;
				}
			}
		}return false;
	}
}
bool Hu()
{
	if(hand.size()<14||hand.size()>18)return false;
	int CardCount=0,i;
	for( i=0;i<hand.size();i++){
		if(hand[i][0]>='a')break;
	}
	CardCount=i;//当前可操作的牌数
	if((CardCount-2)%3!=0)return false;
	int num[6][10]={0};//当前手牌统计
	for(int i=0;i<CardCount;i++){
		pii a=f(hand[i]);
		num[a.first][a.second]++;
		num[a.first][0]++;
	}
	return CheckHu(num[1],9)&&CheckHu(num[2],9)&&CheckHu(num[3],9)&&CheckHu(num[4],4)&&CheckHu(num[5],3);
}

static int count_useful_tile(const tile_table_t &used_table, const useful_table_t &useful_table) {
	int cnt = 0;
	for (int i = 0; i < 34; ++i) {
		tile_t t = all_tiles[i];
		if (useful_table[t]) {
			cnt += 4 - used_table[t];
		}
	}
	return cnt;
}

char chang(int x)
{
	if(x==1)return 'E';
	if(x==2)return 'S';
	if(x==3)return 'W';
	if(x==4)return 'N';
	if(x==5)return 'C';
	if(x==6)return 'F';
	if(x==7)return 'P';
	return ' ';
}
string sw(string x,int t)
{
	string sww;
	if(t==1)
	{
		if(x[0]=='W')
		{
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+='m';
		}
		if(x[0]=='B')
		{
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+='p';
		}
		if(x[0]=='T')
		{
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+='s';
		}
		if(x[0]=='F')
		{
			sww+=chang(x[1]-'0');
			sww+=chang(x[1]-'0');
			sww+=chang(x[1]-'0');
		}
		if(x[0]=='J')
		{
			sww+=chang(x[1]-'0'+4);
			sww+=chang(x[1]-'0'+4);
			sww+=chang(x[1]-'0'+4);
		}
	}
	if(t==2)
	{
		if(x[0]=='W')
		{
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+='m';
		}
		if(x[0]=='B')
		{
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+='p';
		}
		if(x[0]=='T')
		{
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+=x[1];
			sww+='s';
		}
		if(x[0]=='F')
		{
			sww+=chang(x[1]-'0');
			sww+=chang(x[1]-'0');
			sww+=chang(x[1]-'0');
			sww+=chang(x[1]-'0');
		}
		if(x[0]=='J')
		{
			sww+=chang(x[1]-'0'+4);
			sww+=chang(x[1]-'0'+4);
			sww+=chang(x[1]-'0'+4);
			sww+=chang(x[1]-'0'+4);
		}
	}
	if(t==3)
	{
		if(x[0]=='W')
		{
			sww+=x[1]-1;
			sww+=x[1];
			sww+=x[1]+1;
			sww+='m';
		}
		if(x[0]=='B')
		{
			sww+=x[1]-1;
			sww+=x[1];
			sww+=x[1]+1;
			sww+='p';
		}
		if(x[0]=='T')
		{
			sww+=x[1]-1;
			sww+=x[1];
			sww+=x[1]+1;
			sww+='s';
		}
	}
	return sww;
}
char change(int x)
{
	int tmp=x+'0';
	return tmp;
}
string toans(int i,int j)
{
	string tt;
	if(i==1)
	{
		tt+='W';
	}
	if(i==2)
	{
		tt+='T';
	}
	if(i==3)
	{
		tt+='B';
	}
	if(i==4)
	{
		if(j>=5)
		{
			tt+='J';
			j-=4;
		}
		else
		{
			tt+='F';
		}
	}
	tt+=change(j);
	return tt;
}
string cover(int i,int j)
{
	//cout<<"zhelima "<<endl;
	string tmp;
	if(i<4)
	{
		tmp+=change(j);
		if(i==1)tmp+='m';
		if(i==3)tmp+='s';
		if(i==2)tmp+='p';
	}
	else
	{
		if(i==4)
		tmp=chang(j);
		else
		{
			tmp=chang(j+4);
		}

	}
	return tmp;
}

string meow(const hand_tiles_t *hand_tiles, useful_table_t &useful_table)
{
	string tans;
	char buf[64];
		for (tile_t t = TILE_1m; t < TILE_TABLE_SIZE; ++t) {
			if (useful_table[t]) {
				tiles_to_string(&t, 1, buf, sizeof(buf));
				string tmp=buf;
				tans+=tmp+' ';

			}
		}

	return tans;
}

pair<int,string> test_shanten(const char *str)
{
	hand_tiles_t hand_tiles;
	tile_t serving_tile;
	long ret = string_to_tiles(str, &hand_tiles, &serving_tile);
	if (ret != 0) {
		//printf("error at line %d error = %ld\n", __LINE__, ret);
		return make_pair(-1,"");
	}

	char buf[20];
	ret = hand_tiles_to_string(&hand_tiles, buf, sizeof(buf));
	//puts(buf);

	string tans,ans;
	auto display = [](const hand_tiles_t *hand_tiles, useful_table_t &useful_table) {
		char buf[64];
		for (tile_t t = TILE_1m; t < TILE_TABLE_SIZE; ++t) {
			if (useful_table[t]) {
				tiles_to_string(&t, 1, buf, sizeof(buf));
				//printf("%s ", buf);
			}

		}

		tile_table_t cnt_table;
		map_hand_tiles(hand_tiles, &cnt_table);
		count_useful_tile(cnt_table, useful_table);
	   // printf("%d枚\n", count_useful_tile(cnt_table, useful_table));
	};

	//puts(str);
	useful_table_t useful_table/* = {false}*/;
	int ret0;
	ret0 = thirteen_orphans_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);

	//printf("131=== %d shanten\n", ret0);
	if (ret0 != std::numeric_limits<int>::max()) {
		display(&hand_tiles, useful_table);
		tans=meow(&hand_tiles, useful_table);
	}

	if(ret>ret0)
	{
		ret=ret0;
		ans=tans;
	}

	ret0 = seven_pairs_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
	//printf("7d=== %d shanten\n", ret0);
	if (ret0 != std::numeric_limits<int>::max()) {
		display(&hand_tiles, useful_table);
		tans=meow(&hand_tiles, useful_table);
	}

	if(ret>ret0)
	{
		ret=ret0;
		ans=tans;
	}
	ret0 = honors_and_knitted_tiles_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
	//printf("honors and knitted tiles  %d shanten\n", ret0);
	if (ret0 != std::numeric_limits<int>::max()){
		display(&hand_tiles, useful_table);
		tans=meow(&hand_tiles, useful_table);
	}

	if(ret>ret0)
	{
		ret=ret0;
		ans=tans;
	}

	ret0 = knitted_straight_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
	//printf("knitted straight in basic form %d shanten\n", ret0);
	if (ret0 != std::numeric_limits<int>::max()){
		display(&hand_tiles, useful_table);
		tans=meow(&hand_tiles, useful_table);
	}

	if(ret>ret0)
	{
		ret=ret0;
		ans=tans;
	}

	ret0 = basic_form_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
	//printf("basic form %d shanten\n", ret0);
	if (ret0 != std::numeric_limits<int>::max()) {
		display(&hand_tiles, useful_table);
		tans=meow(&hand_tiles, useful_table);

	}

	if(ret>ret0)
	{
		ret=ret0;
		ans=tans;
	}
	//cout<<"??wtf"<<endl;
	return make_pair(ret,ans);
}

string Dapai( vector<string> hand,vector<pair<string, pair<string, int> > > pack,int num[6][10],int myPlayerID,int quan)
{
	//"PENG" "GANG" "CHI"
	//cout<<"wobuhuole"<<endl;
	int map[10][15]={0};
	for(int i=0;i<=5;i++)
	{
		for(int j=0;j<=10;j++)
		{
			map[i][j]=0;
		   // cout<<i<<" "<<j<<endl;
		}

	}
	//cout<<"wobuhuole"<<endl;
	string ming;
   // cout<<"wobuhuole"<<endl;
   // cout<<pack.size()<<endl;
	for(int i=0;i<pack.size();i++)
	{
		ming+='[';
		if(pack[i].first=="PENG")
		{
			ming+=sw(pack[i].second.first,1);
		}
		if(pack[i].first=="GANG")
		{
			ming+=sw(pack[i].second.first,2);
		}
		if(pack[i].first=="CHI")
		{
			ming+=sw(pack[i].second.first,3);
		}
		ming+=']';
	}

//cout<<"wobuhuole"<<endl;

	for(int i=0;i<hand.size();i++)
	{
		if(hand[i][0]=='W')
		{
			int t=hand[i][1]-'0';
			map[1][t]++;
		}
		if(hand[i][0]=='B')
		{
			int t=hand[i][1]-'0';
			map[3][t]++;
		}
		if(hand[i][0]=='T')
		{
			int t=hand[i][1]-'0';
			map[2][t]++;
		}
		if(hand[i][0]=='F')
		{
			int t=hand[i][1]-'0';
			map[4][t]++;
		}
		if(hand[i][0]=='J')
		{
			int t=hand[i][1]-'0';
			map[4][4+t]++;
		}

	}

	int cnt=0;
	int last=0;
	int lastans=0;
	vector <pair<int,int> > lastp;

	for(int i=1;i<=4;i++)
	{
		for(int j=1;j<=9;j++)
		{
			if(map[i][j]!=0)cnt++;
		}
	}

	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=9;j++)
		{
			if(i==4&&j>4)continue;
			if(i==5&&j>3)continue;
			if(num[i][j])
			{

				pair<int,int> t;
				t.first=i;
				t.second=j;
				lastp.push_back(t);
				last++;
			}
			lastans+=num[i][j];
		}
	}

	vector <double> root;
	vector <string> rootn;

	for(int k=1;k<=cnt;k++)
	{

		string tmp;
		int onc=0;
		tmp+=ming;
		int tcnt=0;
		double ans=0;
		int tx=0,ty=0;
		int tmap[6][10]={0};
		for(int i=1;i<=4;i++)
		{
			int ad=0;
			for(int j=1;j<=9;j++)
			{
				if(map[i][j])tcnt++;
				tmap[i][j]=map[i][j];
				if(tcnt==k&&!onc)
				{
					onc=1;
					tmap[i][j]--;
					rootn.push_back(toans(i,j));

				}
				for(int t=1;t<=tmap[i][j];t++)
				{
					ad=1;
					if(i!=4)
					{
						tmp +=change(j);
					}
					else
					{
						tmp += chang(j);
					}
				}
			}
			if(i!=4&&ad)
			{
				if(i==1)tmp+='m';
				if(i==2)tmp+='s';
				if(i==3)tmp+='p';
			}
		}

		char sff[80];
		for(int i=0;i<=tmp.size();i++)
		{
			sff[i]=tmp[i];

		}
		//cout<<tmp<<endl;
		double ret=10086;
		pair<int,string> rt;
		string anse;
		rt=test_shanten(sff);
		ret=rt.first;
		ret+=0.01;
		anse=rt.second;
		//cout<<anse<<endl;
		//cout<<"???"<<endl;
		for(int i=0;i<last;i++)
		{
			//cout<<"???"<<endl;
			pair<int,int> ttmp;
			// cout<<"???"<<endl;
			ttmp=lastp[i];

			// cout<<"???"<<endl;
			string wt=cover(ttmp.first,ttmp.second);

			//cout<<wt<<endl;
			//cout<<anse<<endl;
			 //cout<<"???"<<endl;
			string::size_type position;

			//find 函数 返回jk 在s 中的下标位置
			position = anse.find(wt);
			if (position != anse.npos)
			{
				//cout<<"!!@!@!@#!@#"<<endl;
				ans+=(double)((double)num[ttmp.first][ttmp.second]/(double)lastans)/(double)ret;
			}
		}
		//cout<<endl;

		root.push_back(ans);

	}
	double tmax=-10000.0;
	string tmaxs;
	for(int i=0;i<root.size();i++)
	{
		if(tmax<root[i])
		{
			tmax=root[i];
			tmaxs=rootn[i];
		}
	}
	return tmaxs;
}
int main()
{
	//初始化牌池
	int num[6][10];
	for(int j=1;j<=3;j++)num[j][0]=36;num[4][0]=16;num[5][0]=12;
	for(int i=1;i<=5;i++)for(int j=1;j<=9;j++)num[i][j]=4;

	int turnID,paiqiang=144;//当前牌墙剩余多少牌
	string stmp;
#if SIMPLEIO
	cin >> turnID;
	turnID--;
	getline(cin, stmp);
	for(int i = 0; i < turnID; i++) {
		getline(cin, stmp);
		request.push_back(stmp);
		getline(cin, stmp);
		response.push_back(stmp);
	}
	getline(cin, stmp);
	request.push_back(stmp);
#else
	Json::Value inputJSON;
	cin >> inputJSON;
	turnID = inputJSON["responses"].size();
	for(int i = 0; i < turnID; i++) {
		request.push_back(inputJSON["requests"][i].asString());
		response.push_back(inputJSON["responses"][i].asString());
	}
	request.push_back(inputJSON["requests"][turnID].asString());
#endif

	if(turnID < 2) {
		response.push_back("PASS");
	}
	else {
		int itmp, myPlayerID, quan,hua[4]={0};
		ostringstream sout;
		istringstream sin;
		sin.str(request[0]);
		sin >> itmp >> myPlayerID >> quan;
		sin.clear();
		sin.str(request[1]);
		sin>>itmp;
		for(int j = 0; j < 4; j++) sin >> hua[j],paiqiang-=hua[j];
		for(int j = 0; j < 13; j++) {
			sin >> stmp;
			hand.push_back(stmp);
			pii a=f(stmp);
			num[a.first][0]--;num[a.first][a.second]--;//牌堆减少
		}paiqiang-=4*13;
		string LastCard="",Laststmp="";int Lastuser;
		for(int i = 2; i < turnID; i++) {
			sin.clear();
			sin.str(request[i]);
			sin >> itmp;
			if(itmp == 2) {
				sin >> stmp;  paiqiang--;
				hand.push_back(stmp);
				pii a=f(stmp);
				num[a.first][0]--;num[a.first][a.second]--;
				sin.clear();
				sin.str(response[i]);
				string stmp1,stmp2;
				sin >> stmp1 >> stmp2;
				if(stmp1=="PLAY")hand.erase(find(hand.begin(), hand.end(), stmp2));//直接打出
				else if(stmp1=="GANG"){
					sort(hand.begin(),hand.end());
					for(int i=0;i<4;i++)unhand.push_back(stmp2);  //加入暗杠的手牌
					for(auto i:hand){
						if(i==stmp2)i[0]-='A'-'a';//把杠牌全部变成小写
					}
					sort(hand.begin(),hand.end());
				}
				else if(stmp1=="BUGANG"){
					for(auto i:hand){
						if(i==stmp2)i[0]-='A'-'a';//把杠牌全部变成小写
					}
					for(auto i:pack){
						if(i.first=="PENG"&&i.second.first==stmp2){  //之前一定碰过,直接改
							i.first="GANG";
						}
					}
					sort(hand.begin(),hand.end());
				}
				Lastuser=myPlayerID;
			}
			else {
				sin>>itmp;
				sin>>stmp;
				if(stmp=="BUHUA"){
				    hua[itmp]++; paiqiang--;
				}
				else if(stmp=="DRAW"){
					//空
					paiqiang--;
				}
				else if(stmp=="PLAY"){
					sin>>LastCard;
					pii a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//牌堆减少
				}
				else if(stmp=="PENG"){
					pii a=f(LastCard);
			        num[a.first][0]-=2;num[a.first][a.second]-=2;//牌堆减少
			        if(myPlayerID==itmp){  //相当于我碰牌成功

						pack.push_back({"PENG",{LastCard,fff(myPlayerID,Lastuser)}});
						for(auto i:hand){
						    if(i==LastCard)i[0]-='A'-'a';//把碰牌全部变成小写
					    }LastCard[0]-='A'-'a';
					    hand.push_back(LastCard);

					    sort(hand.begin(),hand.end());
					}
			        sin>>LastCard;
			        a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//牌堆减少
			        if(myPlayerID==itmp){
			        	hand.erase(find(hand.begin(), hand.end(), LastCard));//打出手牌
					}
				}
				else if(stmp=="CHI"){
					pii a=f(LastCard);
			        num[a.first][0]++;num[a.first][a.second]++;
			        if(myPlayerID==itmp){   //我吃的
			        	LastCard[0]-='A'-'a';
						hand.push_back(LastCard);
						sort(hand.begin(),hand.end());
					}
					pii b=a;//备份
			        string zhongCard;
			        sin>>zhongCard;
			        a=f(zhongCard);
			        num[a.first][0]-=3;num[a.first][a.second-1]--;num[a.first][a.second]--;num[a.first][a.second+1]--;
			        if(myPlayerID==itmp){
			        	int dx[]={-1,0,1};
					    for(int i=0;i<3;i++){
					    	if(b.second==a.second+dx[i]){//这张是上家给的牌
							      pack.push_back({"CHI",{zhongCard,i}});
								  continue;//那张之前判过了
							}
					    	string s=ff(a.first,a.second+dx[i]);
						    for(int i=0;i<hand.size();i++){//吃的牌转化为死牌
			            		if(hand[i]==s){
								    hand[i][0]-='A'-'a';
									//sout<<hand[i]<<endl;
									break;//一个就好
								}
							}
						}
						sort(hand.begin(),hand.end());
					}
			        sin>>LastCard;
			        if(myPlayerID==itmp){
			        	hand.erase(find(hand.begin(), hand.end(), LastCard));//打出手牌
					}
					a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//牌堆减少
				}
				else if(stmp=="GANG"){
					if(Laststmp=="DRAW"){
						//暗杠
						//如果是别人 那我什么也做不了
						//如果是自己 上面已经写过了
					}
					else{
						//明杠
						pii a=f(LastCard);
			            num[a.first][0]-=3;num[a.first][a.second]-=3;
					}
				}
				else if(stmp=="BUGANG"){
					string Card;
					sin>> Card;
					pii a=f(Card);
					num[a.first][0]--;num[a.first][a.second]--;
				}

			}
			Laststmp=stmp;Lastuser=itmp;//记录上一次操作
		}

		sin.clear();
		sin.str(request[turnID]);
		//正题开始

		sin >> itmp;
		bool ok=false;//表示是否已经给出回应
		if(itmp == 2) {
			sin>>stmp;//当前摸到的牌
			hand.push_back(stmp);
			pii a=f(stmp);
			num[a.first][0]--;num[a.first][a.second]--;
			sort(hand.begin(),hand.end());
			if(Hu()){
			    	//算番
					MahjongInit();
					vector<string>myhand;
					sort(hand.begin(),hand.end());
					for(auto i:hand){
						if(i[0]<'a')myhand.push_back(i);
						else break;
					}
					for(auto i:unhand){
						myhand.push_back(i);
					}
					bool isZIMO=true,isJUEZHANG=0,isGANG=0,isLAST=0;
					if(num[a.first][a.second]==0)isJUEZHANG=true;
					sin.clear();
					sin.str(request[turnID-1]);
					string stmp1;int ID;
					sin>>stmp1>>ID>>stmp1;
					if(ID==myPlayerID&&(stmp1=="GANG"||stmp1=="BUGANG"))isGANG=1;
					if(paiqiang==0)isLAST=true;
				    vector<pair<int,string>> Fan=MahjongFanCalculator(pack, myhand, stmp, hua[myPlayerID], isZIMO, isJUEZHANG, isGANG, isLAST, myPlayerID, quan);
		            int ans=0;
					for(auto i:Fan){
		            	ans+=i.first;
					}
					if(ans>=8){ok=true;sout<<"HU";}
			}
			if(!ok){
					int CardCount=0,i;
					for( i=0;i<hand.size();i++){
						if(hand[i][0]>='a')break;
					}
					CardCount=i;//当前可操作的牌数
					int mynums[6][10]={0};//当前手牌统计
					for(int i=0;i<CardCount;i++){
						pii a=f(hand[i]);
						mynums[a.first][a.second]++;
						mynums[a.first][0]++;
					}

					int dx[]={0,9,9,9,4,3};
					for(int i=1;i<=5;i++)
					{
						for(int j=1;j<=dx[i];j++)
						{
							if(mynums[i][j]==4){//检测是否有四张一样的
								string s="GANG "+ff(i,j);
								sout<<s;
								ok=true;
								break;
							}
						}
						if(ok==true)break;
					}
					if(!ok)
					{
						//检测是否需要补杠
						stmp[0]-='A'-'a';int cnt=0;
						for(int i=CardCount;i<hand.size();i++){
							if(stmp==hand[i])cnt++;
						}
						if(cnt==3){
							stmp[0]-='a'-'A';//手牌还原
							string s="BUGANG "+stmp;
							sout<<s;
							ok=true;
						}
					}
					if(!ok)
					{
					    //策略
						string Card=Dapai(hand,pack,num,myPlayerID,quan);
						hand.erase(find(hand.begin(), hand.end(), Card));
						sout<<"PLAY "<<Card;ok=true;
					}

			}

		}
		else if(itmp==3){
			int  playerID;
			sin>>playerID>>stmp;
			if(stmp=="PLAY"&&playerID!=myPlayerID){
				int CardCount=0,i;
				for( i=0;i<hand.size();i++){
					if(hand[i][0]>='a')break;
				}
				CardCount=i;//当前可操作的牌数
				int con[6][10]={0};
				for(int i=0;i<CardCount;i++){
					pii a=f(hand[i]);
					con[a.first][a.second]++;
					con[a.first][0]++;
				}
				string Card;sin>>Card;
				pii a=f(Card);
				if(con[a.first][a.second]==2){
					sout<<"PENG ";
					//出牌
					vector<string>uhand=hand;
					uhand.erase(find(uhand.begin(), uhand.end(), Card));
					uhand.erase(find(uhand.begin(), uhand.end(), Card));//删掉两次
					vector<pair<string, pair<string, int> > > upack=pack;
					upack.push_back({"PENG",{Card,fff(myPlayerID,playerID)}});
					string Card=Dapai(uhand,upack,num,myPlayerID,quan);
					sout<<Card;
					ok=true;
				}
				else if(con[a.first][a.second]==3){
				sout<<"GANG";ok=true;
				}
				else if(fff(myPlayerID,playerID)==0&&a.first<=3&&((a.second>2&&con[a.first][a.second-2]>0&&con[a.first][a.second-1]>0)||(a.second>1&&a.second<9&&con[a.first][a.second-1]>0&&con[a.first][a.second+1]>0)||(a.second<=7&&con[a.first][a.second+1]>0&&con[a.first][a.second+2]>0)))
				{
				    sout<<"CHI ";
				    vector<string>uhand=hand;
					vector<pair<string, pair<string, int> > > upack=pack;
					if((a.second>2&&con[a.first][a.second-2]>0&&con[a.first][a.second-1]>0))
					{
					    upack.push_back({"CHI",{ff(a.first,a.second-1),3}});
						string s1=ff(a.first,a.second-2); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						       s1=ff(a.first,a.second-1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						     //s1=ff(a.first,a.second  ); uhand.erase(find(uhand.begin(), uhand.end(), s1));
					    string Card2=Dapai(uhand,upack,num,myPlayerID,quan);
					    sout<<ff(a.first,a.second-1)<<" "<<Card2;ok=true;
					}
					else if((a.second>1&&a.second<9&&con[a.first][a.second-1]>0&&con[a.first][a.second+1]>0))
					{
						upack.push_back({"CHI",{ff(a.first,a.second),2}});
						string s1=ff(a.first,a.second-1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						     //s1=ff(a.first,a.second  ); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						       s1=ff(a.first,a.second+1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
					    string Card2=Dapai(uhand,upack,num,myPlayerID,quan);
					    sout<<ff(a.first,a.second)<<" "<<Card2;ok=true;
					}
					else if((a.second<=7&&con[a.first][a.second+1]>0&&con[a.first][a.second+2]>0))
					{

						upack.push_back({"CHI",{ff(a.first,a.second+1),1}});
						string s1=ff(a.first,a.second+1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						     //s1=ff(a.first,a.second  ); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						       s1=ff(a.first,a.second+2); uhand.erase(find(uhand.begin(), uhand.end(), s1));
					    string Card2=Dapai(uhand,upack,num,myPlayerID,quan);
					    sout<<ff(a.first,a.second+1)<<" "<<Card2;ok=true;
					}
				}
				//
			}
			if(!ok)sout<<"PASS";
		}

		response.push_back(sout.str());
	}

#if SIMPLEIO
	cout << response[turnID] << endl;
#else
	Json::Value outputJSON;
	outputJSON["response"] = response[turnID];
	cout << outputJSON << endl;
#endif
	return 0;
}


/*** Start of inlined file: stringify.cpp ***/
#include <string.h>
#include <algorithm>
#include <iterator>

namespace mahjong {

// 解析牌实现函数
static intptr_t parse_tiles_impl(const char *str, tile_t *tiles, intptr_t max_cnt, intptr_t *out_tile_cnt) {
	//if (strspn(str, "123456789mpsESWNCFP") != strlen(str)) {
	//    return PARSE_ERROR_ILLEGAL_CHARACTER;
	//}

	intptr_t tile_cnt = 0;

#define SET_SUIT_FOR_NUMBERED(value_)       \
	for (intptr_t i = tile_cnt; i > 0;) {   \
		if (tiles[--i] & 0xF0) break;       \
		tiles[i] |= value_;                 \
		} (void)0

#define SET_SUIT_FOR_CHARACTERS()   SET_SUIT_FOR_NUMBERED(0x10)
#define SET_SUIT_FOR_BAMBOO()       SET_SUIT_FOR_NUMBERED(0x20)
#define SET_SUIT_FOR_DOTS()         SET_SUIT_FOR_NUMBERED(0x30)

#define SET_SUIT_FOR_HONOR() \
	for (intptr_t i = tile_cnt; i > 0;) {   \
		if (tiles[--i] & 0xF0) break;       \
		if (tiles[i] > 7) return PARSE_ERROR_ILLEGAL_CHARACTER; \
		tiles[i] |= 0x40;                   \
		} (void)0

#define NO_SUFFIX_AFTER_DIGIT() (tile_cnt > 0 && !(tiles[tile_cnt - 1] & 0xF0))
#define CHECK_SUFFIX() if (NO_SUFFIX_AFTER_DIGIT()) return PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT

	const char *p = str;
	for (; tile_cnt < max_cnt && *p != '\0'; ++p) {
		char c = *p;
		switch (c) {
		case '0': tiles[tile_cnt++] = 5; break;
		case '1': tiles[tile_cnt++] = 1; break;
		case '2': tiles[tile_cnt++] = 2; break;
		case '3': tiles[tile_cnt++] = 3; break;
		case '4': tiles[tile_cnt++] = 4; break;
		case '5': tiles[tile_cnt++] = 5; break;
		case '6': tiles[tile_cnt++] = 6; break;
		case '7': tiles[tile_cnt++] = 7; break;
		case '8': tiles[tile_cnt++] = 8; break;
		case '9': tiles[tile_cnt++] = 9; break;
		case 'm': SET_SUIT_FOR_CHARACTERS(); break;
		case 's': SET_SUIT_FOR_BAMBOO(); break;
		case 'p': SET_SUIT_FOR_DOTS(); break;
		case 'z': SET_SUIT_FOR_HONOR(); break;
		case 'E': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_E; break;
		case 'S': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_S; break;
		case 'W': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_W; break;
		case 'N': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_N; break;
		case 'C': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_C; break;
		case 'F': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_F; break;
		case 'P': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_P; break;
		default: goto finish_parse;
		}
	}

finish_parse:
	// 一连串数字+后缀，但已经超过容量，说明牌过多
	if (NO_SUFFIX_AFTER_DIGIT()) {
		// 这里的逻辑为：放弃中间一部分数字，直接解析最近的后缀
		const char *p1 = strpbrk(p, "mspz");
		if (p1 == nullptr) {
			return PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT;
		}

		switch (*p1) {
		case 'm': SET_SUIT_FOR_CHARACTERS(); break;
		case 's': SET_SUIT_FOR_BAMBOO(); break;
		case 'p': SET_SUIT_FOR_DOTS(); break;
		case 'z': SET_SUIT_FOR_HONOR(); break;
		default: return PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT;
		}

		if (p1 != p) {  // 放弃过中间的数字
			return PARSE_ERROR_TOO_MANY_TILES;
		}

		p = p1 + 1;
	}

#undef SET_SUIT_FOR_NUMBERED
#undef SET_SUIT_FOR_CHARACTERS
#undef SET_SUIT_FOR_BAMBOO
#undef SET_SUIT_FOR_DOTS
#undef SET_SUIT_FOR_HONOR
#undef NO_SUFFIX_AFTER_DIGIT
#undef CHECK_SUFFIX

	*out_tile_cnt = tile_cnt;
	return static_cast<intptr_t>(p - str);
}

// 解析牌
intptr_t parse_tiles(const char *str, tile_t *tiles, intptr_t max_cnt) {
	intptr_t tile_cnt;
	if (parse_tiles_impl(str, tiles, max_cnt, &tile_cnt) > 0) {
		return tile_cnt;
	}
	return 0;
}

// 生成副露
static intptr_t make_fixed_pack(const tile_t *tiles, intptr_t tile_cnt, pack_t *pack, uint8_t offer) {
	if (tile_cnt > 0) {
		if (tile_cnt != 3 && tile_cnt != 4) {
			return PARSE_ERROR_WRONG_TILES_COUNT_FOR_FIXED_PACK;
		}
		if (tile_cnt == 3) {
			if (offer == 0) {
				offer = 1;
			}
			if (tiles[0] == tiles[1] && tiles[1] == tiles[2]) {
				*pack = make_pack(offer, PACK_TYPE_PUNG, tiles[0]);
			}
			else {
				if (tiles[0] + 1 == tiles[1] && tiles[1] + 1 == tiles[2]) {
					*pack = make_pack(offer, PACK_TYPE_CHOW, tiles[1]);
				}
				else if (tiles[0] + 1 == tiles[2] && tiles[2] + 1 == tiles[1]) {
					*pack = make_pack(offer, PACK_TYPE_CHOW, tiles[2]);
				}
				else if (tiles[1] + 1 == tiles[0] && tiles[0] + 1 == tiles[2]) {
					*pack = make_pack(offer, PACK_TYPE_CHOW, tiles[0]);
				}
				else if (tiles[1] + 1 == tiles[2] && tiles[2] + 1 == tiles[0]) {
					*pack = make_pack(offer, PACK_TYPE_CHOW, tiles[2]);
				}
				else if (tiles[2] + 1 == tiles[0] && tiles[0] + 1 == tiles[1]) {
					*pack = make_pack(offer, PACK_TYPE_CHOW, tiles[0]);
				}
				else if (tiles[2] + 1 == tiles[1] && tiles[1] + 1 == tiles[0]) {
					*pack = make_pack(offer, PACK_TYPE_CHOW, tiles[1]);
				}
				else {
					return PARSE_ERROR_CANNOT_MAKE_FIXED_PACK;
				}
			}
		}
		else {
			if (tiles[0] != tiles[1] || tiles[1] != tiles[2] || tiles[2] != tiles[3]) {
				return PARSE_ERROR_CANNOT_MAKE_FIXED_PACK;
			}
			*pack = make_pack(offer, PACK_TYPE_KONG, tiles[0]);
		}
		return 1;
	}
	return 0;
}

// 字符串转换为手牌结构和上牌
intptr_t string_to_tiles(const char *str, hand_tiles_t *hand_tiles, tile_t *serving_tile) {
	size_t len = strlen(str);
	if (strspn(str, "0123456789mpszESWNCFP,[]") != len) {
		return PARSE_ERROR_ILLEGAL_CHARACTER;
	}

	pack_t packs[4];
	intptr_t pack_cnt = 0;
	tile_t standing_tiles[14];
	intptr_t standing_cnt = 0;

	bool in_brackets = false;
	tile_t temp_tiles[14];
	intptr_t temp_cnt = 0;
	intptr_t max_cnt = 14;
	uint8_t offer = 0;

	tile_table_t cnt_table = { 0 };

	const char *p = str;
	while (char c = *p) {
		const char *q;
		switch (c) {
		case ',': {  // 副露来源
			if (!in_brackets) {
				return PARSE_ERROR_ILLEGAL_CHARACTER;
			}
			offer = static_cast<uint8_t>(*++p - '0');
			q = ++p;
			if (*p != ']') {
				return PARSE_ERROR_ILLEGAL_CHARACTER;
			}
			break;
		}
		case '[': {  // 开始一组副露
			if (in_brackets) {
				return PARSE_ERROR_ILLEGAL_CHARACTER;
			}
			if (pack_cnt > 4) {
				return PARSE_ERROR_TOO_MANY_FIXED_PACKS;
			}
			if (temp_cnt > 0) {  // 处理[]符号外面的牌
				if (standing_cnt + temp_cnt >= max_cnt) {
					return PARSE_ERROR_TOO_MANY_TILES;
				}
				// 放到立牌中
				memcpy(&standing_tiles[standing_cnt], temp_tiles, temp_cnt * sizeof(tile_t));
				standing_cnt += temp_cnt;
				temp_cnt = 0;
			}

			q = ++p;
			in_brackets = true;
			offer = 0;
			max_cnt = 4;  // 副露的牌组最多包含4张牌
			break;
		}
		case ']': {  // 结束一副副露
			if (!in_brackets) {
				return PARSE_ERROR_ILLEGAL_CHARACTER;
			}
			// 生成副露
			intptr_t ret = make_fixed_pack(temp_tiles, temp_cnt, &packs[pack_cnt], offer);
			if (ret < 0) {
				return ret;
			}

			q = ++p;
			temp_cnt = 0;
			in_brackets = false;
			++pack_cnt;
			max_cnt = 14 - standing_cnt - pack_cnt * 3;  // 余下立牌数的最大值
			break;
		}
		default: {  // 牌
			if (temp_cnt != 0) {  // 重复进入
				return PARSE_ERROR_TOO_MANY_TILES;
			}
			// 解析max_cnt张牌
			intptr_t ret = parse_tiles_impl(p, temp_tiles, max_cnt, &temp_cnt);
			if (ret < 0) {  // 出错
				return ret;
			}
			if (ret == 0) {
				return PARSE_ERROR_ILLEGAL_CHARACTER;
			}
			// 对牌打表
			for (intptr_t i = 0; i < temp_cnt; ++i) {
				++cnt_table[temp_tiles[i]];
			}
			q = p + ret;
			break;
		}
		}
		p = q;
	}

	max_cnt = 14 - pack_cnt * 3;
	if (temp_cnt > 0) {  // 处理[]符号外面的牌
		if (standing_cnt + temp_cnt > max_cnt) {
			return PARSE_ERROR_TOO_MANY_TILES;
		}
		// 放到立牌中
		memcpy(&standing_tiles[standing_cnt], temp_tiles, temp_cnt * sizeof(tile_t));
		standing_cnt += temp_cnt;
	}

	if (standing_cnt > max_cnt) {
		return PARSE_ERROR_TOO_MANY_TILES;
	}

	// 如果某张牌超过4
	if (std::any_of(std::begin(cnt_table), std::end(cnt_table), [](int cnt) { return cnt > 4; })) {
		return PARSE_ERROR_TILE_COUNT_GREATER_THAN_4;
	}

	// 无错误时再写回数据
	tile_t last_tile = 0;
	if (standing_cnt == max_cnt) {
		memcpy(hand_tiles->standing_tiles, standing_tiles, (max_cnt - 1) * sizeof(tile_t));
		hand_tiles->tile_count = max_cnt - 1;
		last_tile = standing_tiles[max_cnt - 1];
	}
	else {
		memcpy(hand_tiles->standing_tiles, standing_tiles, standing_cnt * sizeof(tile_t));
		hand_tiles->tile_count = standing_cnt;
	}

	memcpy(hand_tiles->fixed_packs, packs, pack_cnt * sizeof(pack_t));
	hand_tiles->pack_count = pack_cnt;
	*serving_tile = last_tile;

	return PARSE_NO_ERROR;
}

// 牌转换为字符串
intptr_t tiles_to_string(const tile_t *tiles, intptr_t tile_cnt, char *str, intptr_t max_size) {
	bool tenhon = false;
	char *p = str, *end = str + max_size;

	static const char suffix[] = "mspz";
	static const char honor_text[] = "ESWNCFP";
	suit_t last_suit = 0;
	for (intptr_t i = 0; i < tile_cnt && p < end; ++i) {
		tile_t t = tiles[i];
		suit_t s = tile_get_suit(t);
		rank_t r = tile_get_rank(t);
		if (s == 1 || s == 2 || s == 3) {  // 数牌
			if (r >= 1 && r <= 9) {  // 有效范围1-9
				if (last_suit != s && last_suit != 0) {  // 花色变了，加后缀
					if (last_suit != 4 || tenhon) {
						*p++ = suffix[last_suit - 1];
					}
				}
				if (p < end) {
					*p++ = '0' + r;  // 写入一个数字字符
				}
				last_suit = s;  // 记录花色
			}
		}
		else if (s == 4) {  // 字牌
			if (r >= 1 && r <= 7) {  // 有效范围1-7
				if (last_suit != s && last_suit != 0) {  // 花色变了，加后缀
					if (last_suit != 4) {
						*p++ = suffix[last_suit - 1];
					}
				}
				if (p < end) {
					if (tenhon) {  // 天凤式后缀
						*p++ = '0' + r;  // 写入一个数字字符
					}
					else {
						*p++ = honor_text[r - 1];  // 直接写入字牌相应字母
					}
					last_suit = s;
				}
			}
		}
	}

	// 写入过且还有空间，补充后缀
	if (p != str && p < end && (last_suit != 4 || tenhon)) {
		*p++ = suffix[last_suit - 1];
	}

	if (p < end) {
		*p = '\0';
	}
	return static_cast<intptr_t>(p - str);
}

// 牌组转换为字符串
intptr_t packs_to_string(const pack_t *packs, intptr_t pack_cnt, char *str, intptr_t max_size) {
	char *p = str, *end = str + max_size;
	tile_t temp[4];
	for (intptr_t i = 0; i < pack_cnt && p < end; ++i) {
		pack_t pack = packs[i];
		uint8_t o = pack_get_offer(pack);
		tile_t t = pack_get_tile(pack);
		uint8_t pt = pack_get_type(pack);
		switch (pt) {
		case PACK_TYPE_CHOW:
			if (p >= end) break;
			*p++ = '[';
			temp[0] = static_cast<tile_t>(t - 1); temp[1] = t; temp[2] = static_cast<tile_t>(t + 1);
			p += tiles_to_string(temp, 3, p, static_cast<intptr_t>(end - p));
			if (p >= end) break;
			*p++ = ',';
			if (p >= end) break;
			*p++ = '0' + o;
			if (p >= end) break;
			*p++ = ']';
			break;
		case PACK_TYPE_PUNG:
			if (p >= end) break;
			*p++ = '[';
			temp[0] = t; temp[1] = t; temp[2] = t;
			p += tiles_to_string(temp, 3, p, static_cast<intptr_t>(end - p));
			if (p >= end) break;
			*p++ = ',';
			if (p >= end) break;
			*p++ = '0' + o;
			if (p >= end) break;
			*p++ = ']';
			break;
		case PACK_TYPE_KONG:
			if (p >= end) break;
			*p++ = '[';
			temp[0] = t; temp[1] = t; temp[2] = t; temp[3] = t;
			p += tiles_to_string(temp, 4, p, static_cast<intptr_t>(end - p));
			if (p >= end) break;
			*p++ = ',';
			if (p >= end) break;
			*p++ = '0' + (is_promoted_kong(pack) ? o | 0x4 : o);
			if (p >= end) break;
			*p++ = ']';
			break;
		case PACK_TYPE_PAIR:
			temp[0] = t; temp[1] = t;
			p += tiles_to_string(temp, 2, p, static_cast<intptr_t>(end - p));
			break;
		default: break;
		}
	}

	if (p < end) {
		*p = '\0';
	}
	return static_cast<intptr_t>(p - str);
}

// 手牌结构转换为字符串
intptr_t hand_tiles_to_string(const hand_tiles_t *hand_tiles, char *str, intptr_t max_size) {
	char *p = str, *end = str + max_size;
	p += packs_to_string(hand_tiles->fixed_packs, hand_tiles->pack_count, str, max_size);
	if (p < end) p += tiles_to_string(hand_tiles->standing_tiles, hand_tiles->tile_count, p, static_cast<intptr_t>(end - p));
	return static_cast<intptr_t>(p - str);
}

}

/*** End of inlined file: stringify.cpp ***/

