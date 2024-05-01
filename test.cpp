#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

/*ゲームについて*/
// プレイヤ人数は2人固定．
// 各プレイヤを1P，2Pと呼ぶ．

/*コードについて*/
// グローバル変数にはできるだけconstexprとstaticを付けた
// boardは局面の意味
// 局面の表現にはbit列のbitset型とarrayのvector<int>型がある
// arrayは「局面のポケットの配列」のことを示す
// listは「なにかのリスト（List型ではない）」を示す

/*パラメータ*/
// 1P, 2Pのポケットの個数（{1Pのpケットの個数, 2Pのポケットの個数}）
// constexpr static int pocket_num_list[2] = {6, 6};
constexpr array<int, 2> pocket_num_list = {6, 6};
// 最初に各ポケットに入っている石の個数
constexpr int default_stone_num = 2;
// 両サイドにある，石を溜めるポケットの個数
// constexpr static int trash_pocket_num = 2;

/*定数関数*/
// dec_numのbit数を数える
constexpr int calculate_bit_length(unsigned int dec_num) {
    int bits = 0;
    while (dec_num > 0) {
        dec_num >>= 1;
        ++bits;
    }
    return bits > 0 ? bits : 1; // 最小値として1を保証
}

/*その他のグローバル変数*/
// 石の総数
constexpr int sum_stone_num =
    (pocket_num_list[0] + pocket_num_list[1]) * default_stone_num;
// ポケットの総数（両サイドのポケットも含める）
constexpr int sum_pocket_num = 2 + pocket_num_list[0] + pocket_num_list[1];
// 現在のターン（false/0：1P，true/1：2P）
static bool current_turn = false;
// １ポケット分のbit数
constexpr int bit_len_of_pocket = calculate_bit_length(sum_stone_num);
// bit列の長さ
constexpr int full_bit_len = sum_pocket_num * bit_len_of_pocket;
// 局面の隣接リスト（key：局面，value：keyの局面に隣接する局面）
unordered_map<bitset<full_bit_len>, set<bitset<full_bit_len>>> adjacent_list;
// keyの局面を計算したかの情報を持つmap
unordered_map<bitset<full_bit_len>, int> confirmed_board;

int main() {
    cout << full_bit_len << "," << bit_len_of_pocket << endl;

    return 0;
}
