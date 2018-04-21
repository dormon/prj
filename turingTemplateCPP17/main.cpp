#include <iostream>

template<typename T>
struct Identity {
  using type = T;
};

// define a type list 
template<typename...>
struct TypeList;

template<typename T, typename... TT>
struct TypeList<T, TT...>  {
  using type = T;
  using tail = TypeList<TT...>;
};

template<>
struct TypeList<> {

};

template<typename List>
struct GetSize;

template<typename... Items>
struct GetSize<TypeList<Items...>> {
  size_t static constexpr value = sizeof...(Items);
};

template<typename... T>
struct ConcatList;

template<typename... First, typename... Second, typename... Tail>
struct ConcatList<TypeList<First...>, TypeList<Second...>, Tail...> {
  using type = typename ConcatList<TypeList<First..., Second...>, 
                                Tail...>::type;
};

template<typename T>
struct ConcatList<T> {
  using type = T;
};

template<typename NewItem, typename List>
struct AppendItem;

template<typename NewItem, typename...Items>
struct AppendItem<NewItem, TypeList<Items...>> {
  using type = TypeList<Items..., NewItem>;
};

template<typename NewItem, typename List>
struct PrependItem;

template<typename NewItem, typename...Items>
struct PrependItem<NewItem, TypeList<Items...>> {
  using type = TypeList<NewItem, Items...>;
};

//std::tuple_element_t<>

template<typename List, int N, typename = void>
struct GetItem {
  static_assert(N > 0, "index cannot be negative");
  static_assert(GetSize<List>::value > 0, "index too high");
  using type = typename GetItem<typename List::tail, N-1>::type;
};

template<typename List>
struct GetItem<List, 0> {
  static_assert(GetSize<List>::value > 0, "index too high");
  using type = typename List::type;
};

template<typename List, template<typename, typename...> class Matcher, typename... Keys>
struct FindItem {
  static_assert(GetSize<List>::value > 0, "Could not match any item.");
  using current_type = typename List::type;
  using type = typename std::conditional_t<Matcher<current_type, Keys...>::value, 
                               Identity<current_type>, // found!
                               FindItem<typename List::tail, Matcher, Keys...>>
      ::type;
};

template<typename List, int I, typename NewItem>
struct ReplaceItem {
  static_assert(I > 0, "index cannot be negative");
  static_assert(GetSize<List>::value > 0, "index too high");
  using type = typename PrependItem<typename List::type, 
                           typename ReplaceItem<typename List::tail, I-1,
                                                NewItem>::type>
      ::type;
};

template<typename NewItem, typename Type, typename... T>
struct ReplaceItem<TypeList<Type, T...>, 0, NewItem> {
    using type = TypeList<NewItem, T...>;
};

enum Direction {
  Left  = -1,
  Right = +1,
};

template<typename OldState, typename Input, typename NewState, 
         typename Output, Direction Move>
struct Rule {
  using old_state = OldState;
  using input     = Input;
  using new_state = NewState;
  using output    = Output;
  Direction static constexpr direction = Move;
};

template<typename Input, typename State, int Position>
struct Configuration {
  using input = Input;
  using state = State;
  int static constexpr position = Position;
};

constexpr int Max(int A,int B){return A>B?A:B;}

template<int n>
struct State {
  int static constexpr value = n;
  char const static* name;
};

template<int n>
char const* State<n>::name = "unnamed";

struct QAccept {
  int static constexpr value = -1;
  char const static* name;
};

struct QReject {
  int static constexpr value = -2;
  char const static* name; 
};

#define DEF_STATE(ID, NAME) \
  using NAME = State<ID>;   \
  NAME :: name = #NAME

template<int n>
struct Input {
  int static constexpr value = n;
  char const static* name;

  template<int... I>
  struct Generate {
      using type = TypeList<Input<I>...>;
  };
};

template<int n>
char const* Input<n>::name = "unnamed";

using InputBlank = Input<-1>;

#define DEF_INPUT(ID, NAME) \
  using NAME = Input<ID>;   \
  NAME :: name = #NAME

template<typename Config, typename Transitions, typename = void> 
struct Controller {
  using config = Config;
  int static constexpr position = config::position;

  using input = typename std::conditional_t<
    static_cast<int>(GetSize<typename config::input>::value) <= static_cast<int>(position),
    AppendItem<InputBlank, typename config::input>,
    Identity<typename config::input>>::type;
  using state = typename config::state;

  using cell = typename GetItem<input, position>::type;

  template<typename Item, typename State, typename Cell>
  struct Matcher {
    using checking_state = typename Item::old_state;
    using checking_input = typename Item::input    ;
    int static constexpr value = std::is_same_v<State, checking_state> && std::is_same_v<Cell,  checking_input>;
  };
  using rule = typename FindItem<Transitions, Matcher, state, cell>::type;

  using new_input  = typename ReplaceItem<input, position, typename rule::output>::type;
  using new_state  = typename rule::new_state;
  using new_config = Configuration<new_input,new_state, Max(position + rule::direction, 0)>;

  using next_step  = Controller<new_config, Transitions>;
  using end_config = typename next_step::end_config;
  using end_input  = typename next_step::end_input;
  using end_state  = typename next_step::end_state;
  int static constexpr end_position = next_step::position;
};

template<typename Input, typename State, int Position, typename Transitions>
struct Controller<Configuration<Input, State, Position>, Transitions, 
                typename std::enable_if_t<std::is_same_v<State, QAccept> || std::is_same_v<State, QReject>>> {
  using config = Configuration<Input, State, Position>;
  int static constexpr position = config::position;
  using input = typename std::conditional_t<
    static_cast<int>(GetSize<typename config::input>::value) <= static_cast<int>(position),
    AppendItem<InputBlank, typename config::input>,
    Identity<typename config::input>>::type;
  using state      = typename config::state;
  using end_config = config;
  using end_input  = input;
  using end_state  = state;
  int static constexpr end_position = position;
};

template<typename Input, typename Transitions, typename StartState>
struct TuringMachine {
  using input       = Input;
  using transitions = Transitions;
  using start_state = StartState;
  using controller  = Controller<Configuration<Input, StartState, 0>, Transitions>;
  using end_config  = typename controller::end_config;
  using end_input   = typename controller::end_input;
  using end_state   = typename controller::end_state;
  int static constexpr end_position = controller::end_position;
};

#include <ostream>

template<>
char const* Input<-1>::name = "_";

char const* QAccept::name = "qaccept";
char const* QReject::name = "qreject";

int main() {
  DEF_INPUT(1, x     );
  DEF_INPUT(2, x_mark);
  DEF_INPUT(3, split );

  DEF_STATE(0, start     );
  DEF_STATE(1, find_blank);
  DEF_STATE(2, go_back   );

  /* syntax:  State, Input, NewState, Output, Move */
  using rules = TypeList< 
      Rule<start     , x         , find_blank, x_mark, Right>,
      Rule<find_blank, x         , find_blank, x     , Right>,
      Rule<find_blank, split     , find_blank, split , Right>,
      Rule<find_blank, InputBlank, go_back   , x     , Left >,
      Rule<go_back   , x         , go_back   , x     , Left >,
      Rule<go_back   , split     , go_back   , split , Left >,
      Rule<go_back   , x_mark    , start     , x     , Right>,
      Rule<start     , split     , QAccept   , split , Left >>;

  /* syntax: initial input, rules, start state */
  using double_it = TuringMachine<TypeList<x, x, x, x, split>, rules, start>;

  static_assert(std::is_same_v<double_it::end_input, 
                       TypeList<x, x, x, x, split, x, x, x, x>>, 
              "Hmm... This is borky!");
  return 0;
}
