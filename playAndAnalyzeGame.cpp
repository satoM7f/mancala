#include <math.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

/*
プレイヤ人数は2人固定．
各プレイヤを1P，2Pと呼ぶ．
*/

constexpr static int pocket_num_1 = 6; // 1Pのポケットの個数
constexpr static int pocket_num_2 = 6; // 2Pのポケットの個数
constexpr static int default_stone_num =
    2; // 最初に各ポケットに入っている石の個数
constexpr static int trash_pocket_num =
    2; // 両サイドにある，石を溜めるポケットの個数

int sum_stone_num =
    (pocket_num_1 + pocket_num_1) * default_stone_num; // 石の総数
int sum_pocket_num =
    trash_pocket_num + pocket_num_1 +
    pocket_num_2; // ポケットの総数（両サイドのポケットも含める）

// 10進数（int型）を2進数（string型）に変換
string dec2bin(int dec) {
    if (dec == 0) return "0";
    constexpr static int max_bit_len =
        100; // bit列の最大の長さ（これを超える長さのbit列は扱えない）
    string bin(bitset<max_bit_len>(dec).to_string());
    // cout << bin << endl;
    bin = bin.substr(bin.find('1'), max_bit_len - bin.find('1'));
    return bin;
}

// 10進数（int型）をstr_n文字の2進数（string型）に変換
string dec2bin_n_dig(int dec, int str_n) {
    string bin_free_deg = dec2bin(dec);
    int precision = str_n - min(str_n, (int)bin_free_deg.size());
    string bin_n_dig = string(precision, '0').append(bin_free_deg);
    return bin_n_dig;
}

/*
// ゲームの状態を表すbit列をテキストの何番目の文字で区切るか示す配列を作成
vevtor<int> makeBitSepLenVec() {
    vector<int> bit_sep_len_vec(sum_pocket_num);
    for (int i = 0; i < sum_pocket_num, i++) {
        bit_sep_len_vec[i] = dec2bin(sum_stone_num).size();
    }
    return bit_sep_len_vec;
}
*/

// bit列（string型）をポケットの配列（vector<int>型）に変換
vector<int> bits2board_array(string bit_text) {
    vector<int> board(sum_pocket_num);

    // bit_len_of_pocket：bit列をポケットごとに分けるときの１ポケットを示すbit列の長さ
    // １ポケットはsum_stone_num個を表現するbit列の分だけbit数を確保すれば十分なので，
    // dec2bin(sum_stone_num)の文字列長が各ポケットを示すbitの長さ
    //
    // sum_stone_numが15個以内の場合の局面を表すbit列のイメージ
    // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
    // |両サイド||1Pのポケット                ||2Pのポケット                 |
    // 各ポケットのbitの長さは4で十分
    // なぜなら１ポケットには多くても15（1111）個しか石が入らないから
    int bit_len_of_pocket = dec2bin(sum_stone_num).size();

    string cliped_bit_text;
    // bit_textをbit_len_of_pocketごとに分割して１０進数変換
    for (int i = 0; i < sum_pocket_num; i++) {
        cliped_bit_text =
            bit_text.substr(i * bit_len_of_pocket, bit_len_of_pocket);
        board[i] = stoi(cliped_bit_text, 0, 2);
    }
    return board;
}

// ポケットの配列（vector<int>型）をbit列（string型）に変換
string board_array2bits(vector<int> board_array) {
    int bit_len_of_pocket = dec2bin(sum_stone_num).size();
    string bit_text = "";
    for (int i = 0; i < board_array.size(); i++) {
        bit_text += dec2bin_n_dig(board_array[i], bit_len_of_pocket);
    }
    return bit_text;
}

// 初期盤面の生成
vector<int> initBoardArray() {
    vector<int> board(sum_pocket_num);
    for (int i = 0; i < trash_pocket_num; i++) board[i] = 0;
    for (int i = trash_pocket_num; i < trash_pocket_num + pocket_num_1; i++)
        board[i] = default_stone_num;
    for (int i = trash_pocket_num + pocket_num_1; i < sum_pocket_num; i++)
        board[i] = default_stone_num;
    return board;
}

// 任意の型のvectorを表示
template <typename T>
void priVec(vector<T> vec, string sep = "\n") {
    cout << "---priVec--->" << endl;
    for (int i = 0; i < vec.size(); i++) cout << vec[i] << sep;
    if (sep != "\n") cout << endl;
    cout << "<---" << endl;
}

// ゲームが終了条件を見たいしているかチェック
bool checkEndGame(string board_bit){
    int bit_len_of_pocket = dec2bin(sum_stone_num).size();
    string check_bit;
    int padding;

    // 1Pが終了条件を満たしているかチェック
    padding = trash_pocket_num * bit_len_of_pocket;
    check_bit = board_bit.substr(padding, bit_len_of_pocket * pocket_num_1);
    // 1が0個すなわちそのプレイヤのポケットの石がすべてなくなったならtrue
    if (count(check_bit.begin(), check_bit.end(), '1') == 0) return true;

    // 2Pが終了条件を満たしているかチェック
    padding = trash_pocket_num * bit_len_of_pocket + bit_len_of_pocket * pocket_num_1;
    check_bit = board_bit.substr(padding, bit_len_of_pocket * pocket_num_2);
    if (count(check_bit.begin(), check_bit.end(), '1') == 0) return true;

    // 終了条件を満たしていないならfalse
    return false;
}

// 局面を表すbit列を受け取って最終的な勝者を返す
int game(string board_bit){
    return winner;
}


int main() {
    
    // cout << dec2bin(10) << endl;

    vector<int> board_array = initBoardArray();
    // vector<int> board_array = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6};
    priVec(board_array, " ");

    string board_bit = board_array2bits(board_array);
    cout << board_bit << endl;

    /*
    vector<int> board_bit_to_array = bits2board_array(board_bit);
    priVec(board_bit_to_array, " ");
    */

    bool is_end = checkEndGame(board_bit);
    cout << is_end << endl;

    return 0;
}