#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <iterator>
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
constexpr array<int, 2> pocket_num_list = {2, 2};
// 最初に各ポケットに入っている石の個数
constexpr int default_stone_num = 1;
// 両サイドにある，石を溜めるポケットの個数
// constexpr static int trash_pocket_num = 2;
// ポケットの個数の多い方（1Pと2Pで大きい値）  三項演算子許して...
constexpr int max_pocket_num = (pocket_num_list[0] < pocket_num_list[1])
                                   ? pocket_num_list[1]
                                   : pocket_num_list[0];

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
// 局面の隣接リスト（key：局面，value：keyの局面に隣接する局面の配列（1Pと2Pで多いポケットの個数分確保））
unordered_map<bitset<full_bit_len>, unordered_set<bitset<full_bit_len>>>
    adjacent_list;
// keyの局面を計算したかの情報を持つmap
unordered_map<bitset<full_bit_len>, bool> confirmed_board;
// 1Pと2Pどちらが勝つか（非不偏ゲームのためPorNではない，L,R,...？）
// static unordered_map<bitset<full_bit_len>, int> winner;

/*bit_len_of_pocket（bit列をポケットごとに分けるときの１ポケットを示すbit列の長さ）について*/
// １ポケットはsum_stone_num個を表現するbit列の分だけbit数を確保すれば十分なので，
// dec2bin(sum_stone_num)の文字列長が各ポケットを示すbitの長さ
//
// sum_stone_numが15個以内の場合の局面を表すbit列のイメージ
// 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
// |1Pのポケット                ||sub||2Pのポケット                 ||sub|
// 各ポケットのbitの長さは4で十分
// なぜなら１ポケットには多くても15（1111）個しか石が入らないから

// 10進数（int型）を2進数（string型）に変換
string dec2bin(int dec) {
    if (dec == 0) return "0";
    constexpr int max_bit_len =
        100; // bit列の最大の長さ（これを超える長さのbit列は扱えない）
    string bin(bitset<max_bit_len>(dec).to_string());
    // cout << bin << endl;
    bin = bin.substr(bin.find('1'), max_bit_len - bin.find('1'));
    return bin;
}

/*
// 10進数（int型）をstr_n文字の2進数（string型）に変換
string dec2bin_n_dig(int dec, int str_n) {
    string bin_free_deg = dec2bin(dec);
    int precision = str_n - min(str_n, (int)bin_free_deg.size());
    string bin_n_dig = string(precision, '0').append(bin_free_deg);
    return bin_n_dig;
}
*/

// 10進数（int型）をstr_n文字の2進数（bitset型）に変換
bitset<bit_len_of_pocket> dec2bin_n_dig(int dec) {
    bitset<bit_len_of_pocket> bin_n_dig(dec);
    return bin_n_dig;
}

// bit列（bitset型）をポケットの配列（vector<int>型）に変換
vector<int> bit2array(bitset<full_bit_len> board_bit) {
    vector<int> board_array(sum_pocket_num);
    string bit_text = board_bit.to_string();
    string cliped_bit_text;

    // bit_textをbit_len_of_pocketごとに分割して１０進数変換
    for (int i = 0; i < sum_pocket_num; i++) {
        cliped_bit_text =
            bit_text.substr(i * bit_len_of_pocket, bit_len_of_pocket);
        board_array[i] = stoi(cliped_bit_text, 0, 2);
    }
    return board_array;
}

// ポケットの配列（vector<int>型）をbit列（string型）に変換
bitset<full_bit_len> array2bit(vector<int> board_array) {
    /// int bit_len_of_pocket = dec2bin(sum_stone_num).size();
    string board_bit = "";
    for (int i = 0; i < board_array.size(); i++) {
        board_bit += dec2bin_n_dig(board_array[i]).to_string();
    }
    return (bitset<full_bit_len>)board_bit;
}

// 初期盤面の生成
// sum_stone_numが15個以内の場合の局面を表すbit列のイメージ
// 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
// |1Pのポケット                ||sid||2Pのポケット                 ||sid|
//
// arrayのイメージ
// {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
// |1Pのポケット     |sd||2Pのポケット     |sd|
vector<int> initBoardArray() {
    vector<int> board(sum_pocket_num);
    for (int i = 0; i < sum_pocket_num; i++) board[i] = default_stone_num;
    // 両サイドのポケットを空にする
    board[pocket_num_list[0]] = 0;
    board[board.size() - 1] = 0;
    return board;
}

// 任意の型のvectorを表示
template <typename T>
void priVec(vector<T> vec, string sep = " ", bool print_deco = true) {
    if (print_deco) cout << "--priVec-->";
    for (int i = 0; i < vec.size(); i++) cout << vec[i] << sep;
    if (sep != "\n") cout << endl;
}

// 任意の型のarrayを表示
template <typename T>
void priAry(array<T, full_bit_len> ary, string sep = " ",
            bool print_deco = true) {
    if (print_deco) cout << "--priVec-->";
    for (int i = 0; i < ary.size(); i++) cout << ary[i] << sep;
    if (sep != "\n") cout << endl;
}

void priBit(bitset<full_bit_len> bit, string sep = " ",
            bool print_deco = true) {
    string bit_string = bit.to_string();
    if (print_deco) cout << "--priBit-->";
    for (int i = 0; i < sum_pocket_num; i++) {
        cout << bit_string.substr(i * bit_len_of_pocket, bit_len_of_pocket)
             << sep;
    }
    cout << endl;
}

// ゲームが終了条件を満たしているかチェック
// bitset型でやったほうが速そうだからvector型は使わない
bool checkEndGame(bitset<full_bit_len> board_bit) {
    // priBit(board_bit); // deb
    bitset<full_bit_len> check_bit = board_bit;

    /*1Pが終了条件を満たしているかチェック*/
    // チェック範囲のビットを残してできるだけ右に詰める
    // check_bit = check_bit >> ((sum_pocket_num - pocket_num_list[0]) *
    // bit_len_of_pocket); priBit(check_bit); //deb
    // .any()：1のビットが一つも存在しなければfalse，一つでも存在すればtrueを返す
    if (!check_bit.any()) {
        // cout << "end" << endl; // deb
        return true;
    }

    // チェックするbit列のリセット
    check_bit = board_bit;

    /*2Pが終了条件を満たしているかチェック*/
    // チェック範囲のビットを残してできるだけ右に詰める
    check_bit = check_bit >> bit_len_of_pocket;
    // priBit(check_bit); //deb
    // チェック範囲のビットを残してできるだけ左に詰める
    check_bit = check_bit << ((sum_pocket_num - pocket_num_list[1]) *
                              bit_len_of_pocket);
    // priBit(check_bit); // deb
    // cout << (sum_pocket_num - pocket_num_list[1]) << endl; // deb
    // .any()：1のビットが一つも存在しなければfalse，一つでも存在すればtrueを返す
    if (!check_bit.any()) {
        // cout << "end" << endl; // deb
        return true;
    }

    // 上のどちらの終了条件も満たしていないならfalse
    return false;
}

// ある局面（vector）と選択するポケットの番号から着手した結果の局面（bitset型）を返す
// ポケットの番号は自分の一番左のポケットが０番，１番，２番と数える
どちらのターンかわかる必要がある（先頭に1bitつける？）
bitset<full_bit_len> moveBoard(vector<int> current_board,
                               int select_pocket_index) {
    vector<int> next_board_array = current_board;
    int remove_stone_num = next_board_array[select_pocket_index];
    // indexのポケットの石を全て取り除く
    next_board_array[select_pocket_index] = 0;
    // 取り除いた石の個数回，順番に各ポケットの石の個数を+1する
    for (int i = 1; i <= remove_stone_num; i++) {
        next_board_array[(select_pocket_index + i) % sum_pocket_num]++;
    }
    return array2bit(next_board_array);
}

// ある局面（vector型）から可能な着手を一手したときの着手後の盤面を全て返す
unordered_set<bitset<full_bit_len>> calcMoveBoard(
    vector<int> current_board_array) {
    // 一手先の盤面のセット
    unordered_set<bitset<full_bit_len>> next_board_set = {};

    // next_board_listは遷移の個数個の要素を持つ
    // 遷移の個数はそのターンのプレイヤーのポケットの石の個数
    // pocket_num_list[current_turn]がそれ
    for (int select_pocket_i = 0;
         select_pocket_i < pocket_num_list[current_turn]; select_pocket_i++) {
        // select_pocket_i番目のポケットに石があるならそこに着手した結果の盤面をnext_board_setに入れる
        if (current_board_array[select_pocket_i] != 0) {
            next_board_set.insert(
                moveBoard(current_board_array, select_pocket_i));
            // cout << "calcMoveBoardの結果 "; // deb
            // priBit(moveBoard(current_board_array, select_pocket_i), " ", false); //deb
        }
    }
    return next_board_set;
}

// ある局面（bitset型）から可能な着手を一手したときの着手後の盤面を全て返す
unordered_set<bitset<full_bit_len>> calcMoveBoard(
    bitset<full_bit_len> current_board_bit) {
    return calcMoveBoard(bit2array(current_board_bit));
}

// ある局面から一手着手する，を繰り返す
// グローバル変数の隣接リストができる
// 再帰が深くなりすぎるとメモリが心配
void playGame(bitset<full_bit_len> board_bit) {
    cout << ">>>>> ";  // deb
    priBit(board_bit); // deb

    // その局面が終了条件を満たしているか判定
    if (checkEndGame(board_bit) || confirmed_board[board_bit]) {
        cout << "return (end)board" << endl;
        return;
    }

    cout << ">>>   ";  // deb
    priBit(board_bit); // deb

    // 次の局面を計算し隣接リストに登録
    adjacent_list[board_bit] = calcMoveBoard(board_bit);
    // 一度計算したところを覚えておく
    confirmed_board[board_bit] = 1;
    // 一手着手したらターンを進める
    current_turn ^= 1;

    // 遷移した局面からさらにplayGameする
    for (bitset<full_bit_len> adjacent_board : adjacent_list[board_bit]) {
        cout << "隣接";                     // deb
        priBit(adjacent_board, " ", false); // deb
        playGame(adjacent_board);
    }

    return;
}

// mapをイテレータにして中を見る
// 隣接リスト用
void priMapBit(unordered_map<bitset<full_bit_len>,
                             unordered_set<bitset<full_bit_len>>>
                   adjacent_list) {
    for (auto itr = adjacent_list.begin(); itr != adjacent_list.end(); ++itr) {
        // itr->firstでkey
        cout << itr->first << ":";
        // itr->secondでvalue
        for (auto adjacent_board : itr->second) {
            cout << adjacent_board << ",";
        }
        cout << endl;
    }
}

int main() {
    // cout << dec2bin(10) << endl;

    vector<int> board_array = initBoardArray();
    // priVec(board_array, " ");

    bitset<full_bit_len> board_bit = array2bit(board_array);
    // priBit(board_bit);

    playGame(board_bit);

    priMapBit(adjacent_list);

    /*
    vector<int> board_bit_to_array = bits2board_array(board_bit);
    priVec(board_bit_to_array, " ");
    */

    // bool is_end = checkEndGame(board_bit);
    // cout << is_end << endl;

    return 0;
}