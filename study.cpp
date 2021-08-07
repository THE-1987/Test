// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <typeinfo>
// #include <vector>
// #include <algorithm>

using namespace std;

/**
 * label文の仕様を確認したい
 * [stmt.label]
 */
// // label文は以下の3つの形式のいずれかである
// // 1. (identifierラベル) attribute-specifier-seq_opt identifier : statement
// // 2. (caseラベル) attribute-specifier-seq_opt case constant-expression : statement
// // 3. (defaultラベル) attribute-specifier-seq_opt default : statement
// // statementは必ず";"を含むため，label文の":"の後には必ず文が存在しなければならない
// void f()
// {
//         // label: // NG，ラベルに続く文が無い
// }
//
// // identifierラベルで宣言するidentifierは，goto文でしか使えない
// // caseラベル，defaultラベルは，switch文の中でしか使えない
// void g()
// {
//         label_1:; // OK，null文
//         label_2:{} // OK，複合文
//
//         // label_1; // NG，identifierはgoto文でしか使えない
//
//         switch(0){
//         case 0:; // OK
//         default:; // OK
//         }
// }
//
// 関数内の2つのラベルが，同じidentifierを持ってはいけない
// 即ち，ラベルを再宣言することはできない
// void h()
// {
//         label:;
//         label:; // NG，ラベルは再宣言できない
// }
//
// identifierは，label文で宣言する前に使用できる
// void i()
// {
//         goto label; // labelを宣言する前に使用できる
//
//         label:;
// }


/**
 * goto文の仕様を確認したい
 * [stmt.goto]
 */
// goto文は，label文によって指定された場所に無条件に制御を移す
// goto文は，参照先のラベルと同じ関数内でなければならない
// void f()
// {
//         label:; // labelという名前のlabel文
//
//         goto label;
// }


/**
 * conditionが宣言の時の仕様を確認したい
 * [stmt.pre]
 */
// conditionは以下の2つの形式のいずれかである
// 1. expression
// 2. attribute-specifier-seq_opt decl-specifier-seq declarator brace-or-equal-initializer
// 2の形式のconditionは，宣言である
// conditionで，関数と配列の宣言をしてはならない
// conditionのdecl-specifier-seqは，クラス及び列挙型を定義してはならない
// switch文を除いてconditionの値は，宣言された変数を文脈的にboolに変換(contextually converted to bool)した値になる
// switch文の場合，conditionの値は，宣言された変数が整数型or列挙型ならばその値になり，
// それ以外は宣言された変数を整数型or列挙型に暗黙的に変換した値になる
// もし変換がill-formedの場合，プログラムはill-formedとなる


/**
 * range-based for文の正体を確認したい
 * [stmt.ranged]
 */
// range-based for文は次の形式である
// for (init-statement_opt for-range-declaration : for-range-initializer) statement
// range-based for文は次と同値である：
// {
//         init-statement_opt
//         auto&& range=for-range-initializer;
//         auto begin=begin-expr;
//         auto end=end-expr;
//         for (; begin!=end; ++begin){
//                 for-range-declaration= *begin;
//                 statement
//         }
// }
// range, begin, endはあくまでも説明用の変数である
// もしfor-range-initializerがexpressionである場合，'()'で囲まれているものとみなされる
// したがってカンマによる複数のinit-declaratorの宣言をしても意味をなさない
// for-range-declarationのdecl-specifier-seqは，クラス及び列挙型を定義してはならない
// begin-exprとend-exprは以下の場合分けで決定される：
// 1. for-range-initializerがサイズNの配列の場合，
//    begin-expr=range, end-expr=range+N となる
//    サイズが未知の場合はill-formedとなる
//
// 2. for-range-initializerがクラスの場合，
//    非修飾名でそのクラススコープにメンバとしてbegin, endがいるか，メンバアクセスに従って名前探索をする
//    もし両方とも少なくとも一つ名前が見つかった場合は
//    begin-expr=range.begin(), end-expr=range.end() となる
//
// 3. それ以外の場合，関連名前空間(associated namespaces)から，begin, endを名前探索し
//    begin-expr=begin(range), end-expr=end(range) となる


/**
 * for文の正体を確認したい
 * [stmt.for]
 */
// for文は次の形式である
// for (init-statement condtion_opt ; expression_opt) statement
// conditionとexpressionは省略可能である
// init-statementが宣言の場合，宣言された名前のスコープはstatementの終わりまで拡張される
// for文は次と同値である：
// {
//         init-statement
//         while (condition_opt){
//                 statement
//                 expression_opt;
//         }
// }
// ただし，以下の点に注意すること：
// 1. init-statementで宣言された名前と，conditionで宣言された名前と同じ宣言領域にある
// 2. statementにcontinue(statement内の他の繰り返し文に囲まれていない)が含まれている場合，conditionの再評価の前にexpressionが実行される
// 3. conditionが省略された場合，while(condition)はwhile(true)と同値になる


/**
 * while文の正体を確認したい
 * [stmt.while]
 */
// while文は次の形式である
// while (condition) statement
// conditionが宣言の場合，宣言された変数のスコープはstatementの終わりまで拡張される
// while文は次と同値である：
// label:
// {
//         if(condition){
//                 statement
//                 goto label;
//         }
// }
// conditionで生成される変数は毎ループで生成され破棄される


/**
 * クラス宣言について確認したい
 * [class.name],[basic.lookup.elab],[basic.scope.pdecl]
 */
// // クラス宣言すると，クラス名をスコープに導入する
// // クラス名は外側のスコープの名前を隠す
// class X{};
//
// void X(int);
//
// void f()
// {
//         class X; // ::Xを隠す
//         X(0); // NG，void ::X(int)も隠されているため参照されない
// }
//
// // もしクラス名が宣言されたスコープ内に，同名の変数，関数，enumが宣言された場合はどちらの宣言もスコープに存在することになる
// // この時，複雑型指定子(elaborated-type-specifier)を使用した時のみ，そのクラスを参照することができる
// class Y{};
//
// void Y(int);
//
// void g()
// {
//         Y* py1; // NG
//         class Y* py2; // OK，::Yが参照される
//         Y(0); // OK
// }
//
// // class-key identifier; だけの宣言は次のいずれかである：
// // 1. 同じスコープ内にあるクラス名の再宣言
// // 2. クラス宣言としての前方宣言
// class Z{};
//
// void h()
// {
//         class Z; // ローカルクラスZの前方宣言，::Zを隠す
//
//         Z* pz; // ローカルクラスZを参照する
//
//         class Z{}; // ローカルクラスZの定義
//
//         class Z; // ローカルクラスZの再宣言
// }
//
// // 複雑型指定子は宣言の一部として使用することができる
// // もし，複雑型の名前がスコープにある場合，その名前を参照する
// // この点において，クラス宣言とは異なる
// class W{};
//
// void i(int W)
// {
//         class W* pw=new class W; // ::Wを参照
//         W=1; // 引数Wを参照
// }
//
// // 複雑型指定子は宣言の一部として使用することができる
// // もし，複雑型の名前が名前探索で前方宣言が見つからない場合，
// // その複雑型指定子はそのクラス名のクラス宣言となり，以下の場合分けで名前を導入する：
// // 1. 複雑型指定子が名前空間スコープの関数定義の，decl-specifier-seq or parameter-declaration-clauseで使用されている場合は，
// //    その名前空間スコープにそのクラス名を導入する
// // 2. friend宣言を除いてそれ以外の場合は，複雑型指定子を宣言の一部として使用している宣言，
// //    を囲っている最小の名前空間 or ブロックスコープにそのクラス名を導入する
// class S* pu; // グローバル名前空間にクラスSが導入される
//
// namespace N{
//         class T* j(class U*) // 名前空間NにクラスT，Uが導入される
//         {
//                 class V* pv; // ローカルクラスVが導入される
//
//                 return nullptr;
//         }
// }
//
// N::T* k(N::U*); // クラスN::T，N::Uが参照される


/**
 * noreturn属性の使い方を確認したい
 * [dcl.attr.noreturn],[dcl.pre]
 */
// // noreturn属性は関数がreturnしないことを指定する属性である
// // noreturnは関数の宣言で使用できる
// // noreturnは宣言中一度しか現れてはならない
// // [[noreturn, noreturn]] // NG，2度現れている
// // void f1();
//
// [[noreturn]] [[noreturn]] // OK
// void f2();
//
// // noreturnを指定する関数は，その最初の宣言でnoreturnを指定しなくてはならない
// // void g1(); // NG，最初の宣言にnoreturnが指定されていない
//
// [[noreturn]]
// void g1();
//
// [[noreturn]]
// void g2();
//
// void g2(); // OK，最初以外の宣言はnoreturnが有っても無くても良い
//
// [[noreturn]]
// void g2();
//
// // もし，ある関数がある翻訳単位でnoreturnで宣言されていて，
// // 別の翻訳単位でnoreturnで宣言されていない場合プログラムはill-formedである(NDR)
// // noreturnを指定した関数がreturnした場合，挙動は未定義である
// // 例外によって抜けるのは問題ない
// [[noreturn]]
// void g2(int a)
// {
//         if(a){
//                 throw "Hello"; // OK
//         }
//         // a==0の場合はUB(Undefined Behavior)
// }
//
// // 用途としては以下の使い方がある
// // 例外送出する関数をラッピングして，かつ何もreturnしない関数を定義するとコンパイラは警告を出す
// // その例外送出する関数にnoreturnを指定するとその警告を抑制することができる
// [[noreturn]]
// void h()
// {
//         throw "Hello";
// }
//
// int i()
// {
//         h();
//         // iは絶対にreturnしないが，コンパイラはそれを認識できない
//         // hをnoreturnで指定することでコンパイラに最適化のヒントを与える
// }


/**
 * 引数の無いreturn文(return statement with no operand)が使用できる場所を確認したい
 * [stmt.return]
 */
// // 引数の無いreturn文は以下の関数でしか使用できない
// // ・戻り値の型が(cv)void型
// // ・コンストラクタ
// // ・デストラクタ
// // 上記の関数の本体の最後は，引数の無いreturn文が書かれたことと同じになる
// void f()
// {
//         // return; と同じになる
// }


/**
 * 不完全型の宣言・定義について確認したい
 *[basic.types],[dcl.array]
 */
// // 宣言されているが定義されていないクラス，一部のenum，サイズが未知な配列や要素が不完全型の配列は，
// // 不完全に定義されたオブジェクト型(incompletely-defined object type)という
// // 不完全に定義されたオブジェクト型は，サイズやレイアウトは未知である
// // 不完全に定義されたオブジェクト型と，void型(cv)を不完全型という
// // オブジェクトは，不完全型を持ったまま定義されてはならない
// // クラス型は翻訳単位のある地点では不完全であり，その後完全になることができる
// // その場合クラスの型は変わらない
// class X; // 不完全型
// // X x; // エラー
// class X{}; // Xは完全になる
//
// // 配列は，宣言時に要素として不完全なクラス型を含むことができ，不完全となることがある
// // もしその翻訳単位内で，その後そのクラス型が完全になった場合，その時点でその配列も完全になる
// // この場合配列の型は変わらない
// class Y;
// using T=Y [10]; // OK，不完全型
// // T arr1Y; // エラー，不完全型Yの配列の定義
// class Y{}; // クラスYが完全になる
// T arr2Y; // OK，完全型Yの配列の定義
//
// // 配列は，宣言時にサイズを未知とすることができ，翻訳単位のある地点では不完全となることがある
// // そのような配列は後で完全にすることができる
// // その場合，その配列はそれら2点で異なる型である
// // int arr1[]; // エラー，不完全な配列は定義できない
// extern int arr2[]; // OK，宣言はできる
// int arr2[10]; // OK，完全な配列になる
//
// // また完全な配列を宣言した後に，同じスコープで，サイズを省略した定義を行うと前方宣言のサイズに従う
// extern char arr3[5];
// char arr3[]; // OK，前方宣言に従いサイズ5の配列になる
//
// // サイズが未知の配列へのポインタ型，またはtypedefでサイズが未知な配列型と定義された型は，完全になることができない
// int (*p)[]; // pは完全な配列へのポインタになることはできない
// typedef int ARR[]; // ARRは完全になることはできない


/**
 * mutable指定子の使い方を確認したい
 * [dcl.stc]
 */
// // mutable指定子は非staticメンバにしか使用できない
// // constまたは参照型には使用できない
// // mutable指定をすることで，constメンバ関数からでも値を変更することが可能になる
// struct X{
//         mutable int m_a; // OK
// //         mutable char const m_c; // エラー，constメンバ
// //         mutable float& m_f; // エラー，参照型
// //         static mutable unsigned m_u; // エラー，staticメンバ
//         void f() const
//         {
//                 m_a=1; // OK，constメンバ関数内でも変更可
//         }
// };


/**
 * using宣言でテンプレート識別子が使えないことを確認したい
 */
// // using宣言でテンプレート識別子を使うことはできない
// namespace NS{
//         template<typename T>
//         void f(T);
// }
//
// // using NS::f<char> // エラー
// using NS::f; // OK


/**
 * using司令が使用できるスコープを確認したい
 */
// // using司令(directive)は名前空間スコープかブロックスコープで使用できる，クラススコープでは使えない
// namespace NS{}
//
// struct X{
//  // using namepace NS; // エラー
// };


/**
 * 宣言領域，ポテンシャルスコープ，スコープの違いを確認したい
 * [basic.scope.declarative]
 */
// // 宣言領域(Declarative region)：全ての名前は，宣言領域と呼ばれるプログラムテキストの一部に導入される
// // 宣言領域は，名前が有効であるプログラムの最大部分である
// // 即ち，非修飾名を使用して同じエンティティを参照することができる
// //
// // スコープ(Scope)：一般に，各名前はスコープと呼ばれるプログラムテキスト内の一部(連続している必要はない)でのみ有効である
// //
// // ポテンシャルスコープ(Potential scope)：基本的にはスコープと同じであるが，
// // 内側に同じ名前の他の宣言がある場合は，内側の宣言領域で宣言されている名前のポテンシャルスコープは，
// // 外側の宣言領域で宣言されている名前のスコープからは除外される
// //
// int a=1987;
//
// void f()
// {
//         int b=a, a;
//         a=0;
// }
// // 最初のaの宣言領域は，この例全体を包含する
// // 最初のaのポテンシャルスコープはaの直後からこの例の最後までだが，
// // そのaのスコープは，','から'}'の間を含まない
// // 2つ目のa(';'の前の方)の宣言領域は，'{'から'}'の間である
// // 2つ目のaのポテンシャルスコープは，bの宣言を含まない
// // 2つ目のaのポテンシャルコープとスコープは同じになる


/**
 * デフォルト引数の細かい仕様を確認したい
 */
// // デフォルト引数は，引数(対応する引数)なしで関数が呼ばれる度に評価される
// // テンプレートパラメータパック，関数パラメータパックにデフォルト引数は指定できない
// // また関数ポインタ，関数への参照，typedef宣言でも使うことはできない
// // デフォルト引数の先行の引数は，デフォルト引数と同じスコープのため，他の名前を隠してしまう
// // またデフォルト引数に他の引数を指定することはできない(odr-used)
// // デフォルト引数にローカル変数(static変数も含む)は使用できない(odr-used)
// template<typename ... Ts =char> // エラー
// void f(T ... ts=0); // エラー
// void (*p)(int=0); // エラー
// typedef void f(int=0); // エラー，usingを使った宣言(エイリアス宣言)も当然エラー
//
// int a;
//
// // void f1(int a, int=a); // エラー，グローバル変数aを先行の引数が隠す
// void f2(int a, int=sizeof(a)); // OK，規格ではOKだが，GCCもClangもエラー
//
// void f3()
// {
//         int b;
//         static char c;
//
//         // void h1(int=b); // エラー
//         // void h2(char=c); // エラー
//         void h4(int=sizeof(b)); // OK，規格ではOKだが，GCCもClangもエラー
// }
//
// // 非テンプレート関数に対して，同じスコープ内で再宣言でデフォルト引数を付け加えることができる
// // また異なるスコープ内の宣言のデフォルト引数は完全に区別され，
// // 内側のスコープ内の再宣言は外側のスコープの宣言からデフォルト引数を取得しない(逆も然り)
// // 一方，グローバルスコープ内で宣言された名前のポテンシャルスコープは宣言場所から翻訳単位の終わりまでなので，
// // 異なる翻訳単位の宣言のデフォルト引数は当然お互いに影響しない
// // 関数宣言のデフォルト引数の後続の引数は，その宣言 or それ以前の宣言で指定されなければならない
// // ただし，パラメータパックから展開された場合 or 関数パラメータパックである場合は除かれる(省略記号"..."は引数ではないのでOK)
// // また同じスコープ内で既にデフォルト引数を指定されている引数に，再宣言でデフォルト引数を指定することはできない(たとえ同一の値であっても)
// // 異なる翻訳単位でinline関数が定義された場合，蓄積されたデフォルト引数は各翻訳単位の最後で同じでなければならない(NDR)
// // 演算子関数は関数呼び出し演算子を除いてデフォルト引数を指定できない
// void F(int, int=0);
// void F(int=0, int); // OK
// // void F(int, int=0); // エラー，デフォルト引数は再宣言できない
//
// // void G(char='a', char); // エラー，後続の引数にデフォルト引数が指定されていない
//
// void H(int =0, ...); // OK，後続が省略記号
//
// template<typename T=int, typename U> // OK，関数宣言ではない
// void tF(T, U=0);
//
// // template<typename T, typename U>
// // void tF(T=0, U); // エラー，テンプレート関数は条件の対象外
//
// template<typename T=char, typename ... Us> // OK，関数宣言ではない
// void tG(T='a', Us ...); // OK，後続がパラメータパック
//
// void I(char, int=0);
//
// void block()
// {
//         // void I(char='a', int); // エラー，外側の宣言のデフォルト引数は考慮されない
// }
//
// // 関数のfriend宣言でデフォルト引数を指定する場合，以下の2つの条件が満たされなければならない：
// // 1. 宣言は定義である
// // 2. その関数の宣言は，その翻訳単位内ではその宣言唯一つである
// この条件から，ローカルクラス内での関数のfriend宣言ではデフォルト引数を指定できないことがわかる
// struct X_friend{
//         // friend void f1(int=0); // エラー，定義でない
//         friend void f2(X={}){}
// };
//
// // void f2(X); // エラー，前方宣言でfriend指定されていて，デフォルト引数が指定されている
//
// // 非テンプレートクラスのメンバ関数に対して，デフォルト引数はクラス外定義でもクラス内の宣言に対して追加することができる
// // その際クラス外定義でのデフォルト引数の指定によって
// // メンバ関数がデフォルト，コピー，ムーブコンストラクタ，またはコピー，ムーブ代入演算子になってはならない
// // テンプレートクラスのメンバ関数の場合，デフォルト引数はメンバ関数の最初の宣言で指定されなければならない
// // デフォルト引数にthisポインタは使用できない
// // デフォルト引数に非staticメンバを指定することはできない(未評価式であっても)
// // ただしメンバアクセスでメンバへのポインタを形成するために使用する場合はOK(この例外はよくわからん)
// // 仮想関数のデフォルト引数は継承されない，デフォルト引数は仮想関数を呼び出すオブジェクトの型によって決定される
// struct Base{
//         int a;
//         static int b;
//
//         // Base(int);
//         void f1(char, int=0);
//         // void f2(int=a); // エラー
//         // void f3(int=sizeof(a)); // エラー
//         void f4(int=b); // OK
//         // void g(Base* =this); // エラー
//         virtual void h(int=1987)=0;
// };
//
// // Base::Base(int=0){} // エラー，この宣言(定義)でデフォルトコンストラクタになった
//
// void Base::f1(char='a', int){} // OK
// // void Base::f4(int=0){} // エラー，クラススコープ内の宣言で既に指定されている
//
// struct Derived:Base{
//         void h(int) override;
// };
//
// void temp1()
// {
//         Derived d;
//         Base& b=d;
//         // d.h(); // エラー，デフォルト引数は継承されない
//         b.h(); // OK，Derived::h(1987)が呼ばれる
// }
//
// // using宣言はそれまで蓄積されたデフォルト引数も導入する
// // その後，その関数がその名前空間内で再宣言されデフォルト引数が追加された場合，
// // その再宣言以降，そのデフォルト引数もそのusing宣言のスコープで認識されるようになる
// namespace NS{
//         void f_using(char, int=0);
// }
//
// using NS::f_using;
//
// void temp2()
// {
//         f_using('a'); // OK
// }
//
// namespace NS{
//         void f_using(char='a', int){}
// }
//
// void temp3()
// {
//         f_using(); // OK，NS::f_using('a', 0)が呼ばれる
// }


/**
 * explicit指定子の仕様を確認したい
 */
// explicit指定子はコンストラクタか変換関数にのみ指定できる
// struct X{
//         explicit X()=default;
//         explicit X(int){}
//         explicit X(X const&)=default; // コピーコンストラクタも OK
//         explicit X(X&&)=default; // ムーブコンストラクタも OK
//
//         explicit operator int()
//         {
//                 return 1987;
//         }
// };
//
// explicit指定されたコンストラクタと変換関数は直接初期化，明示的なキャストでないと呼び出されない
// X x1=1; // エラー，コピー初期化
// X x2={1}; // エラー，コピー初期化
// X x3{1}; // OK
// X x4(1); // OK
// // X x5({1}); // Clangでは警告を出す，GCCではOK
// // X x6=x3; // エラー
// X x7{x3}; // OK
// X x8=X{1}; // OK
// X x9=static_cast<X>(1); // OK
//
// void f(X);
// // f(x3); // エラー
// f(X{x3}); // OK
// X x10{};
// X x11({}); // OK，ただしX(int)が呼ばれていて，intを{}で値初期化している
// // もしX(int)が宣言されていない場合，コピー or ムーブコンストラクタが呼ばれる
// // その際引数部分で X const& x ={} or X&& x={} とリスト初期化が行われる
// // この時，一時オブジェクトtの生成にて X t={} が行われるが，デフォルトコンストラクタがexplicitなのでエラーとなる
//
// int a1=x3; // エラー
// int a2{x3}; // OK
// int a3=int{x3}; // OK
// int a4=static_cast<int>(x3); // OK


/**
 * 変換関数の細かい仕様を確認したい
 */
// クラスのメンバとしてしか宣言できない，staticメンバにはできない
// メンバとして継承できる，仮装関数にできる
// T型への変換関数operator T() の型は，T(void)となる
// 関数型と配列型への変換関数は宣言することができない
// 自分自身の型(cvr)，自分自身の基本クラス型(cvr)，void型(cv)への変換関数は宣言可能だが，基本的に使われることはない
// これらの型への変換には，標準型変換が用いられる
// しかし基本クラスの仮想変換関数を用いると，使用できる場合がある
// struct Y;
//
// struct X{
//         virtual operator Y()=0;
// };
//
// struct Y: X{
//         // operator void(void)(); // エラー
//         // operator int[1](); // エラー
//         operator Y const volatile&(); // OK，しかし基本的に使われることはない
//         operator X(); // OK，しかし基本的に使われることはない
//         operator void(); // OK，しかし基本的に使われることはない
//         operator Y() override
//         {
//                 return Y();
//         }
// };
//
// Y y1;
// Y y2=y1; // Y::operator Y()は呼ばれない
// X& x=y1;
// Y y3=x; // オーバーライドしたY::operator Y()が呼ばれる


/**
 * ネストクラスの仕様を確認したい
 */
// // ネストクラスのスコープは，外側のクラススコープである
// // 即ち名前探索の際も，外側のクラススコープが影響する
// // ネストクラスは外側のクラスの非staticメンバを通常通りに使うことはできない
// // 理由として，クラスのメンバの使用には，オブジェクトがないと使用できないからである
// // 即ち外側のクラスのthisポインタを経由して行う操作は，ネストクラス側からは通常のように使用することはできない
// // ネストクラスのメンバ関数，staticメンバは外側のクラスの外側，同じ名前空間内で定義できる
// // ネストクラスは定義を外側のクラスの外側，同じ名前空間内で定義できる
// char a;
//
// struct Outer{
//         char a;
//         static int b;
//
//         struct Inner1{
//                 static int c;
//                 void f(Outer& t)
//                 {
//                         // a='a'; // エラー，Outer::a
//                         t.a='a'; // OK
//                         ::a='a'; // OK，グローバル変数
//                         sizeof(a); // OK，sizeofのオペランドは未評価式
//                         b=1987; // OK
//                 }
//
//                 void g();
//         };
//
//         struct Inner2;
// };
//
// int Outer::Inner1::c=1213; // OK
// void Outer::Inner1::g(){} // OK
// struct Outer::Inner2{}; // OK
//
// // ネストクラスから外側のクラスへアクセスする方法として，thisポインタをネストクラスに渡す方法がある
// class X{
// private:
//         class Y{
//         private:
//                 X& m_x;
//         public:
//                 Y(X& x):m_x{x}{}
//         };
//
//         Y m_y;
// public:
//         X():m_y{*this}{} // ネストクラスのオブジェクト生成時にthisを渡す
// };


/**
 * ローカルクラスの仕様を確認したい
 */
// // ローカルクラスのスコープは，それを囲っているブロックスコープである
// // 囲っている関数の外側の名前へのアクセスは，その関数と同じになる
// // 囲っている関数の自動変数は使用できない(odr-usable)，typedefやstatic変数は使用できる
// // ローカルクラスはテンプレート宣言できない，メンバーテンプレートもできない
// // メンバ関数はクラス内定義しなくてはならない，クラス外での定義はできない
// // staticメンバ変数を持つことはできない
// // ローカルクラス内で宣言されたネストクラスは，そのローカルクラス内，またはそのローカルクラスと同じスコープ内で定義できる
// // ローカルクラス内のネストクラスもまたローカルクラスである
// struct X;
//
// void f()
// {
//        char a;
//        static int b;
//       
//        struct X; // ローカルクラスXの前方宣言，::X を隠す
//        struct Y;
//
//        struct X{
//                void f()
//                {
//                        // a='a'; // エラー
//                        b=1987; // OK
//                 }
//                 struct Y; // ネストクラスYの前方宣言，Xの外側のローカルクラスYとは異なる
//         };
//
//         struct X::Y{}; // OK，囲っているクラスXと同じスコープで定義できる
// }


/**
 * friend指定子でのクラス定義内宣言・定義について確認したい
 */
// friend関数のクラス内定義可能条件は，以下の3つの条件が満たされることと同値：
// 1. クラスが非ローカルクラスである
// 2. 関数名は非修飾名である
// 3. 関数は名前空間スコープを持つ
// ただし，friend宣言で定義された関数はADL(Argument-dependent name lookup)を使わないと呼び出すことができない
// それ以外の名前探索で関数を参照する方法はない
// friend宣言がその関数の最初の宣言の場合，リンケージはそれが属する名前空間のリンケージに従う
// それ以外の場合，即ち前方宣言がされている場合は前方宣言のリンケージに従う
// friend宣言での名前の使用にはアクセス指定子が影響する
// class X{
//         void g();
// };
//
// class Y{
//         // friend void ::f(){}; // エラー，非修飾名でないと定義できない
//         friend void f(){} // OK，ただしfを呼び出す方法は存在しない
//         friend void X::g(); // エラー，X::gはクラスXのprivateメンバ
// };
//
// void i()
// {
//         // ローカルクラス内でのfriend宣言で非修飾名を使った場合，
//         // 名前探索においてローカルクラスの定義されている関数外のスコープは考慮されない
//         // その宣言が関数の場合，前方宣言が必要であり
//         // その宣言がクラスの場合，クラス名はローカルクラスの名前と解釈される
//         class X{
//                 // friend void h(); // エラー，前方宣言が必要
//                 // たとえグローバル名前空間にhが宣言されていたとしても考慮されない，その場合修飾名::hを使う必要がある
//
//                 friend class Y; // OK，ローカルクラスYを宣言
//         };
// }


/**
 * inline関数内のローカルstatic変数について確認したい
 */
// 外部リンケージを持つinline関数は，全ての翻訳単位に対して同じアドレスを持つ
// そのため外部リンケージを持つinline関数内のローカルstatic変数は，常に同一のオブジェクトを参照する
// クラス定義内で定義された関数は暗黙でinlineとなるので，メンバ関数内のローカルstatic変数も同様


/**
 * friend宣言でのinline指定の仕様を確認したい
 */
// // friend宣言でinline指定する場合，その宣言は定義であるか，またはinline指定された前方宣言を持たなくてはいけない
// struct X{
//     friend inline void f();
// };
//
// void f(){}
// // ill-formedだが，GCC, Clang, VC++いずれもエラーは出力せず


/**
 * 非inline関数を定義後，それをinlineとして再宣言できないことを確認したい
 */
// void f(){}
//
// inline void f(); // ill-formed
// // NDR(No Diagnostic is Required)のためエラーを出すかはコンパイラ依存
// // GCC, VC++ではエラーは出ず，Clangではエラーが出る


/**
 * 名前空間のメンバが外側で定義できることを確認したい
 */
// namespace A{
//         extern int a;
//         int b;
// }
//
// // 名前空間に既に宣言されているオブジェクトの"定義"ができるのであって，"宣言"ができるのではない
// int A::a; // OK
// // int A::b; // エラー，既に定義されている
// // int A::c; // エラー，Aにcというメンバは宣言されていない


/**
 * 基底クラスへ accessible である場所Rを確認したい
 * (Rでaccessible -> Rで派生クラスの参照・ポインタを基底クラスのそれに変換可能)
 */
// // 条件1：
// // 基底クラスのpublicメンバが目的の派生クラスのpublicである
// // Rは任意である
// struct X{};
//
// struct Y1:public X{};
//
// // 条件2：
// // 基底クラスのpublicメンバが目的の派生クラスのprivate or protectedである
// // Rは目的の派生クラスのメンバ or friendで発生している
// struct Y2_private:private X{
//         X& x2=*this; // OK
// };
//
// struct Y2_protected:protected X{
//         X& x2=*this; // OK
// };
//
// // 条件3：
// // 基底クラスのpublicメンバが目的の派生クラス，の派生クラスのprivate or protectedである
// // Rは目的の派生クラス，の派生クラスのメンバ or friendで発生している
//
// struct Y3:protected X{} y3;
//
// struct Z3_private:private Y3{
//         X& x3=y3; // OK
// };
//
// struct Z3_protected:protected Y3{
//         X& x3=y3; // OK
// };
//
// // 条件4：
// // 基底クラスと目的の派生クラスの間に次の条件を満たすクラスSが存在する
// // SはRで基底クラスにaccessibleであり，目的の派生クラスはRでSにaccessibleである
// struct S:private X{
//         friend class Y4_friend;
// };
//
// struct Y4_friend:private S{
//         X& x4=*this; // OK
//
//         // 基底クラスへaccessibleである時，基底クラスがaccess可能なメンバにもaccessibleである
//         // 従ってXのinjected-class-nameにもaccessibleとなる
// };
//
// struct Y4_nofriend:private S{
//         // X& x4_1=*this; // エラー
//
//         // Xのinjected-class-nameにもinaccessibleである
// };


/**
 * injected-class-nameを確認したい
 */
// class X{};
//
// class Y:private X{};
//
// class Z:public Y{
//         // 自分自身の injected-class-name は必ず public
//         // X の injected-class-name は Y で private になる
//         // そのため X の injected-class-name は Z で inaccessible なメンバになる
//
//         // X x1; // エラー
//
//         ::X x2; // OK
// };


/**
 * static_castの細かい挙動を確認したい
 * [expr.static.cast]
 */
// struct X{
//         X(int=0){}
//         operator int()
//         {
//                 return 1987;
//         }
// };
//
// // 型Tに対して T t(e) と宣言できる場合，式eは型Tに明示的に変換可能である
// // この時Tが参照型の場合は，一時オブジェクトtをそのまま使うことと同じになる
// // それ以外の場合，結果のオブジェクトは式eから直接初期化される
// X x;
// int a;
// x=static_cast<X>(1987); // OK
// a=static_cast<int>(x); // OK
//
// // 任意の式は明示的に(cv)void型へ変換可能である
// // 用途としては，未使用の変数に対する警告を避けるためなど
// static_cast<void>(0); // OK
// static_cast<void>(1+1); // OK
// static_cast<void>(x); // OK


/**
 * 関数ポインタをconst_castできないことを確認したい
 */
// void f(char* s)
// {
//         cout << s << endl;
// }
//
// 関数ポインタに対して，const_castを使うことはできない
// 未定義動作を引き起こす値に繋がってしまうため使うことはできない
// 同様な理由でメンバ関数ポインタ，関数への参照にも使えない
// 当然，仮引数のconstや，constメンバ関数のconstを変更することもできない
// using T=void (*)(char*);
// using U=void (*)(char const*);
//
// T p=&f;
// T const q=const_cast<T const>(p); // エラー
// U r=const_cast<U>(p); // エラー


/**
 * void (cv)*とのキャストについて確認したい
 */
// 任意のポインタはvoid (cv)*へ暗黙に変換可能
// 逆にvoid (cv)*はキャストを用いて任意のポインタ(cvは同じで)に変換可能
// T (cv)* → void (cv)* → T (cv)* としても値は保持される
// char c='0';
// void* p=&c;
// char* q=p; // エラー，暗黙には変換できない
// cout << static_cast<char*>(p) << endl;


/**
 * 関数の引数の型のルールを確認したい
 */
// void f(char);
// void g(char const);
//
// 関数はC言語との互換性のために，"関数の型"として引数のトップレベルのconst修飾を無視する
// 従ってトップレベルをconst修飾しても関数の型は変わらない，そのためオーバーロードも起きない
// しかし，これは引数自体の型を変換している訳ではないため，引数のconstの性質はそのまま残る
// if(typeid(f)==typeid(g)){
//         cout << "Same type" << endl;
// }


/**
 * 純粋仮想関数(デストラクタ)のオーバーライドと挙動を確認したい
 */
// 仮想デストラクタは，派生クラスのデストラクタに自動的にオーバーライドされる
// 純粋仮想関数は，たとえ定義を持っていても，仮想関数呼び出しされた時の挙動は未定義である
// 純粋仮想関数は，関数呼び出しをされる時は定義を持ってなくてはいけない
// 基底クラスの仮想デストラクタは，派生クラスの仮想デストラクタに，非仮想関数呼び出しをされる(らしい)
// 従って，純粋仮想デストラクタは定義を持たなくてはいけない
// struct X{
//         virtual void f()=0;
//         virtual ~X()=0;
// };
//
// void X::f()
// {
//         cout << "pure in Base" << endl;
// }
//
// X::~X()
// {
//         cout << "Base" << endl;
//         X::f();
// }
//
// struct Y: X{
//         void f() override
//         {
//                 cout << "pure in Derived" << endl;
//         }
//
//         ~Y() override
//         {
//                 cout << "Derived" << endl;
//                 f();
//         }
// };
//
// Y y;
// X& x=y;
// x.~X(); // ~Y()が呼ばれ，~X()が非純粋仮想関数呼び出しされる


/**
 * 参照修飾された非staticメンバ関数とcv修飾のみされた非staticメンバ関数では，
 * オーバーロードできないことを確認したい(volatileは無し)
 */
// 参照修飾されたメンバ関数とcv修飾のみされたメンバ関数では相性が悪い(オーバーロードできない)
// これらが混在するクラスは，定義できない
// 参照修飾されたもの同士，cv修飾のみされたもの同士は相性が良い
// struct X{
//         void F()
//         {
//                 cout << "none" << endl;
//         }
//
//         void F()const
//         {
//                 cout << "const" << endl;
//         }
//
//         void F()&
//         {
//                 cout << "&" << endl;
//         }
//
//         void F()&&
//         {
//                 cout << "&&" << endl;
//         }
//
//         void F()const&
//         {
//                 cout << "const&" << endl;
//         }
// }; // エラー


/**
 * 暗黙的に定義されるコピーコンストラクタの引数は，
 * メンバが持つコピーコンストラクタの引数から影響を受ける事の確認をしたい
 */
// 暗黙的に定義されるコピーコンストラクタの引数は，
// メンバが持つコピーコンストラクタの引数から影響を受ける
// struct Y{
//         Y()=default;
//         Y(Y&)=default;
// };
//
// struct X{
//         Y a;
// };
//
// Yのコピーコンストラクタが Y::Y(Y const&) または Y::Y(Y const volatile&) である場合は，X::(X const&) になる
// それ以外は X::X(X&) になる
// X const x1;
//
// X x2(x1); // エラー


/**
 * マクロを使わずに固定長配列のサイズを求めたい
 */
// template<typename T, size_t N>
// size_t Size(T const (&)[N])
// {
//         return N;
// }


/**
 * クラス内で定義されたscoped enumの呼び出しの書き方を確認したい
 */
// struct Base{
//         enum E {alpha, beta}; // unscoped enum
//         enum class Eclass{ first, second }; // scoped enum
// };
//
// cout << Base::alpha << endl; // OK
// cout << Base::E::beta << endl; // OK
// cout << static_cast<int>(Base::Eclass::second) << endl; // OK，この書き方以外NG


/**
 * 無名名前空間の挙動を確かめたい
 */
// 無名名前空間の中で宣言された全ての名前は内部リンケージをもつ
// namespace{
//         char c;
//         extern int m; // たとえextern宣言していても内部リンケージとなる
// }
//
// namespace N{
//         static union{ // 名前空間スコープでの無名共用体はstaticを指定しなくてはならない
//                 char s;
//                 int i;
//         };
//
//         namespace{ // inline無名名前空間との違いはほぼない
//                 char c; // 上のcと衝突しない
//
//                 union{ // 無名名前空間内での無名共用体はstaticをつけなくて良い(GCCではエラー，ClangではOK)
//                         char us;
//                         int ui;
//                 };
//         }
// }


/**
 * メンバ関数ポインタの使用例を確認したい
 */
// void F(Base the, int (Base::*f)() const)
// {
//         cout << "---here is in F-----" << endl;
//
//         cout << (the.*f)() << endl;
// }


/**
 * 関数ポインタの使用例を確認したい
 */
// void F(char const* (*pf)())
// {
//         cout << (*pf)() << endl; // OK
//         cout << pf() << endl; // OK，間接参照演算子*を使わなくてもOK
// }


/**
 * cvr修飾された非staticメンバ関数の暗黙の引数の型を確認したい(volatileは無し)
 */
// struct X {
//         int data;
//
//         X(int a=0): data(a){}
//
//         void F_none()
//         {
//                 cout << "none; " << data << endl;
//         }
//
//         void F_const()const
//         {
//                 cout << "const; " << data << endl;
//         }
//
//         void F_Lref()&
//         {
//                 cout << "&; " << data << endl;
//         }
//
//         void F_Rref()&&
//         {
//                 cout << "&&; " << data << endl;
//         }
//
//         void F_ConstRef()const&
//         {
//                 cout << "const&;" << data << endl;
//         }
// };
//
// union None{
//         void (X::*cpy)();
//
//         void (*F1)(X& s);
//         void (*F2)(X&& s);
// } u1;
//
// union Const{
//         void (X::*cpy)()const;
//
//         void (*F3)(X const& s);
// } u2;
//
// union Lref{
//         void (X::*cpy)()&;
//
//         void (*F1)(X& s);
// } u3;
//
// union Rref{
//         void (X::*cpy)()&&;
//
//         void (*F2)(X&& s);
// } u4;
//
// union ConstantRef{
//         void (X::*cpy)()const&;
//
//         void (*F3)(X const& s);
// } u5;
//
// u1.cpy=&X::F_none;
// u2.cpy=&X::F_const;
// u3.cpy=&X::F_Lref;
// u4.cpy=&X::F_Rref;
// u5.cpy=&X::F_ConstRef;
//
// X x1;
// X const x2;
//
// (*u1.F1)(x1);
// (*u1.F2)(X(1));
//
// (*u2.F3)(x2);
// (*u2.F3)(X(1));
//
// (*u3.F1)(x1);
//
// (*u4.F2)(X(1));
//
// (*u5.F3)(x2);
// (*u5.F3)(X(1));


/**
 * 無名unionの仕様を確認したい
 */
// 無名unionは，メンバ関数を持てないためアクセス指定子も指定できない
// 無名unionは，非staticメンバ変数しか持てない
// 名前空間スコープで宣言する際はstaticを指定しなくてはいけない
// void f()
// {
//         union {
//                 char c;
//                 int i;
//         };
// }


/**
 * (i/o)stringstreamの使い方を確認したい
 */
// char str[]={"1987 hoge 3.14"};
// int m;
// char s[10]={};
// float f;
//
// // istringstreamはsscanfに代替するもの
// // 以下はsscanf(str, "%d %s %f", &m, s, &f)してるのと同じ
// // 型が一致していないとエラーになる
// istringstream iss(str);
// iss >> m;
// iss >> s;
// iss >> f;
//
// cout << m << '\n';
// cout << s << '\n';
// cout << f << endl;
//
// // ostringstreamはsprintfに代替するもの
// // 以下はsprintf(str, "%d %s %f", m, s, f)してるのと同じ
// ostringstream oss;
// oss << m << ' ' << s << ' ' << f;
//
// cout << oss.str() << endl;


/**
 * (i/o)fstream の使い方を確認したい
 */
// string str;
// ifstream ifs("abc.txt");
// ofstream ofs("def.txt");
//
// while(!getline(ifs, str).eof()){
//         cout << str << endl;
//         ofs << str << endl;
// }


/**
 * 特殊メンバ関数の呼び出しを確認したい
 */
// const& による一時オブジェクトの生成とコピー時省略を確認したい
// struct X{
//         X()
//         {
//                 cout << "Default Constructor." << endl;
//         }
//
//         X(X const&)
//         {
//                 cout << "Copy Constructor." << endl;
//         }
//
//         X& operator=(X const&)
//         {
//                 cout << "Copy assignment operator." << endl;
//
//                 return *this;
//         }
//
//         X(X&&)
//         {
//                 cout << "Move Constructor." << endl;
//         }
//
//         X& operator=(X&&)
//         {
//                 cout << "Move assignment operator." << endl;
//
//                 return *this;
//         }
//
//         ~X()
//         {
//                 cout << "Destructor." << endl;
//         }
// };
//
// 以下 const& による一時オブジェクトの生成とコピー時省略の確認
// // デフォルトコンストラクタ
// X x1;
//
// // コピーコンストラクタ
// X x2(x1), x3=x1;
//
// // ムーブコンストラクタ
// X x4(move(x2)), x5=static_cast<X&&>(x3);
//
// // コピー演算子
// x4=x5;
//
// // ムーブ演算子
// x4=move(x5);
//
// // コピー省略
// // rvalueで初期化しようとしてもムーブコンストラクタが呼ばれず，デフォルトコンストラクタが呼ばれる
// X x6=X();
//
// // const& による一時オブジェクトの生成
// // 一時オブジェクトが生成され，const& はそれを参照している?
// X const& x7=X(x1);
// X const& x8=2;


/**
 * ポインタのcv変換の仕様を確認したい
 */
// char** から char const** には変換できない
// 仮にできた場合，char*がchar constのポインタを持ててしまうような例が作れる
// 詳細は N4659 に記載されている
// char const a='a';
// char *p;
//
// char const** q=&p;
// *q=&a;       // p=&a が実現できてしまう
// *p='A';
//
// char *p;
// char *const* q=&p; // これは問題ない

