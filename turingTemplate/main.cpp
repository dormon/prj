#include <iostream>

template<bool C, typename A, typename B>
struct Conditional {
    typedef A type;
};

template<typename A, typename B>
struct Conditional<false, A, B> {
    typedef B type;
};

template<typename...>
struct ParameterPack;

template<bool C, typename = void>
struct EnableIf { };

template<typename Type>
struct EnableIf<true, Type> {
    typedef Type type;
};

template<typename T>
struct Identity {
    typedef T type;
};

// define a type list 
template<typename...>
struct TypeList;

template<typename T, typename... TT>
struct TypeList<T, TT...>  {
    typedef T type;
    typedef TypeList<TT...> tail;
};

template<>
struct TypeList<> {

};

template<typename List>
struct GetSize;

template<typename... Items>
struct GetSize<TypeList<Items...>> {
    enum { value = sizeof...(Items) };
};

template<typename... T>
struct ConcatList;

template<typename... First, typename... Second, typename... Tail>
struct ConcatList<TypeList<First...>, TypeList<Second...>, Tail...> {
    typedef typename ConcatList<TypeList<First..., Second...>, 
                                Tail...>::type type;
};

template<typename T>
struct ConcatList<T> {
    typedef T type;
};

template<typename NewItem, typename List>
struct AppendItem;

template<typename NewItem, typename...Items>
struct AppendItem<NewItem, TypeList<Items...>> {
    typedef TypeList<Items..., NewItem> type;
};

template<typename NewItem, typename List>
struct PrependItem;

template<typename NewItem, typename...Items>
struct PrependItem<NewItem, TypeList<Items...>> {
    typedef TypeList<NewItem, Items...> type;
};

template<typename List, int N, typename = void>
struct GetItem {
    static_assert(N > 0, "index cannot be negative");
    static_assert(GetSize<List>::value > 0, "index too high");
    typedef typename GetItem<typename List::tail, N-1>::type type;
};

template<typename List>
struct GetItem<List, 0> {
    static_assert(GetSize<List>::value > 0, "index too high");
    typedef typename List::type type;
};

template<typename List, template<typename, typename...> class Matcher, typename... Keys>
struct FindItem {
    static_assert(GetSize<List>::value > 0, "Could not match any item.");
    typedef typename List::type current_type;
    typedef typename Conditional<Matcher<current_type, Keys...>::value, 
                                 Identity<current_type>, // found!
                                 FindItem<typename List::tail, Matcher, Keys...>>
        ::type::type type;
};

template<typename List, int I, typename NewItem>
struct ReplaceItem {
    static_assert(I > 0, "index cannot be negative");
    static_assert(GetSize<List>::value > 0, "index too high");
    typedef typename PrependItem<typename List::type, 
                             typename ReplaceItem<typename List::tail, I-1,
                                                  NewItem>::type>
        ::type type;
};

template<typename NewItem, typename Type, typename... T>
struct ReplaceItem<TypeList<Type, T...>, 0, NewItem> {
    typedef TypeList<NewItem, T...> type;
};

enum Direction {
    Left = -1,
    Right = 1
};

template<typename OldState, typename Input, typename NewState, 
         typename Output, Direction Move>
struct Rule {
    typedef OldState old_state;
    typedef Input input;
    typedef NewState new_state;
    typedef Output output;
    static Direction const direction = Move;
};

template<typename A, typename B>
struct IsSame {
    enum { value = false }; 
};

template<typename A>
struct IsSame<A, A> {
    enum { value = true };
};

template<typename Input, typename State, int Position>
struct Configuration {
    typedef Input input;
    typedef State state;
    enum { position = Position };
};

template<int A, int B>
struct Max {
    enum { value = A > B ? A : B };
};

template<int n>
struct State {
    enum { value = n };
    static char const * name;
};

template<int n>
char const* State<n>::name = "unnamed";

struct QAccept {
    enum { value = -1 };
    static char const* name;
};

struct QReject {
    enum { value = -2 };
    static char const* name; 
};

#define DEF_STATE(ID, NAME) \
    typedef State<ID> NAME ; \
    NAME :: name = #NAME ;

template<int n>
struct Input {
    enum { value = n };
    static char const * name;

    template<int... I>
    struct Generate {
        typedef TypeList<Input<I>...> type;
    };
};

template<int n>
char const* Input<n>::name = "unnamed";

typedef Input<-1> InputBlank;

#define DEF_INPUT(ID, NAME) \
    typedef Input<ID> NAME ; \
    NAME :: name = #NAME ;

template<typename Config, typename Transitions, typename = void> 
struct Controller {
    typedef Config config;
    enum { position = config::position };

    typedef typename Conditional<
        static_cast<int>(GetSize<typename config::input>::value) 
            <= static_cast<int>(position),
        AppendItem<InputBlank, typename config::input>,
        Identity<typename config::input>>::type::type input;
    typedef typename config::state state;

    typedef typename GetItem<input, position>::type cell;

    template<typename Item, typename State, typename Cell>
    struct Matcher {
        typedef typename Item::old_state checking_state;
        typedef typename Item::input checking_input;
        enum { value = IsSame<State, checking_state>::value && 
                       IsSame<Cell,  checking_input>::value
        };
    };
    typedef typename FindItem<Transitions, Matcher, state, cell>::type rule;

    typedef typename ReplaceItem<input, position, typename rule::output>::type new_input;
    typedef typename rule::new_state new_state;
    typedef Configuration<new_input, 
                          new_state, 
                          Max<position + rule::direction, 0>::value> new_config;

    typedef Controller<new_config, Transitions> next_step;
    typedef typename next_step::end_config end_config;
    typedef typename next_step::end_input end_input;
    typedef typename next_step::end_state end_state;
    enum { end_position = next_step::position };
};

template<typename Input, typename State, int Position, typename Transitions>
struct Controller<Configuration<Input, State, Position>, Transitions, 
                  typename EnableIf<IsSame<State, QAccept>::value || 
                                    IsSame<State, QReject>::value>::type> {
    typedef Configuration<Input, State, Position> config;
    enum { position = config::position };
    typedef typename Conditional<
        static_cast<int>(GetSize<typename config::input>::value) 
            <= static_cast<int>(position),
        AppendItem<InputBlank, typename config::input>,
        Identity<typename config::input>>::type::type input;
    typedef typename config::state state;

    typedef config end_config;
    typedef input end_input;
    typedef state end_state;
    enum { end_position = position };
};

template<typename Input, typename Transitions, typename StartState>
struct TuringMachine {
    typedef Input input;
    typedef Transitions transitions;
    typedef StartState start_state;

    typedef Controller<Configuration<Input, StartState, 0>, Transitions> controller;
    typedef typename controller::end_config end_config;
    typedef typename controller::end_input end_input;
    typedef typename controller::end_state end_state;
    enum { end_position = controller::end_position };
};

#include <ostream>

template<>
char const* Input<-1>::name = "_";

char const* QAccept::name = "qaccept";
char const* QReject::name = "qreject";

int main() {
    DEF_INPUT(1, x);
    DEF_INPUT(2, x_mark);
    DEF_INPUT(3, split);

    DEF_STATE(0, start);
    DEF_STATE(1, find_blank);
    DEF_STATE(2, go_back);

    /* syntax:  State, Input, NewState, Output, Move */
    typedef TypeList< 
        Rule<start, x, find_blank, x_mark, Right>,
        Rule<find_blank, x, find_blank, x, Right>,
        Rule<find_blank, split, find_blank, split, Right>,
        Rule<find_blank, InputBlank, go_back, x, Left>,
        Rule<go_back, x, go_back, x, Left>,
        Rule<go_back, split, go_back, split, Left>,
        Rule<go_back, x_mark, start, x, Right>,
        Rule<start, split, QAccept, split, Left>> rules;

    /* syntax: initial input, rules, start state */
    typedef TuringMachine<TypeList<x, x, x, x, split>, rules, start> double_it;
    static_assert(IsSame<double_it::end_input, 
                         TypeList<x, x, x, x, split, x, x, x, x>>::value, 
                "Hmm... This is borky!");
}
