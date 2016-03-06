#include <boost/coroutine2/all.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include <sstream>

using namespace std::placeholders;

typedef boost::coroutines2::coroutine<std::string> coro_t;

// deliver each line of input stream to sink as a separate string
void readlines(coro_t::push_type& sink,std::istream& in){
    std::string line;
    while(std::getline(in,line))
        sink(line);
}

void tokenize(coro_t::push_type& sink, coro_t::pull_type& source){
    // This tokenizer doesn't happen to be stateful: you could reasonably
    // implement it with a single call to push each new token downstream. But
    // I've worked with stateful tokenizers, in which the meaning of input
    // characters depends in part on their position within the input line.
    for(std::string line:source){
        std::string::size_type pos=0;
        while(pos<line.length()){
            if(line[pos]=='"'){
                std::string token;
                ++pos;              // skip open quote
                while(pos<line.length()&&line[pos]!='"')
                    token+=line[pos++];
                ++pos;              // skip close quote
                sink(token);        // pass token downstream
            } else if (std::isspace(line[pos])){
                ++pos;              // outside quotes, ignore whitespace
            } else if (std::isalpha(line[pos])){
                std::string token;
                while (pos < line.length() && std::isalpha(line[pos]))
                    token += line[pos++];
                sink(token);        // pass token downstream
            } else {                // punctuation
                sink(std::string(1,line[pos++]));
            }
        }
    }
}

void only_words(coro_t::push_type& sink,coro_t::pull_type& source){
    for(std::string token:source){
        if (!token.empty() && std::isalpha(token[0]))
            sink(token);
    }
}

void trace(coro_t::push_type& sink, coro_t::pull_type& source){
    for(std::string token:source){
        std::cout << "trace: '" << token << "'\n";
        sink(token);
    }
}

struct FinalEOL{
    ~FinalEOL(){
        std::cout << std::endl;
    }
};

void layout(coro_t::pull_type& source,int num,int width){
    // Finish the last line when we leave by whatever means
    FinalEOL eol;

    // Pull values from upstream, lay them out 'num' to a line
    for (;;){
        for (int i = 0; i < num; ++i){
            // when we exhaust the input, stop
            if (!source) return;

            std::cout << std::setw(width) << source.get();
            // now that we've handled this item, advance to next
            source();
        }
        // after 'num' items, line break
        std::cout << std::endl;
    }
}

int main()
{
    // For example purposes, instead of having a separate text file in the
    // local filesystem, construct an istringstream to read.
    std::string data(
        "This is the first line.\n"
        "This, the second.\n"
        "The third has \"a phrase\"!\n"
        );

    {
        std::cout << "\nfilter:\n";
        std::istringstream infile(data);
        coro_t::pull_type reader(std::bind(readlines, _1, std::ref(infile)));
        coro_t::pull_type tokenizer(std::bind(tokenize, _1, std::ref(reader)));
        coro_t::pull_type filter(std::bind(only_words, _1, std::ref(tokenizer)));
        coro_t::pull_type tracer(std::bind(trace, _1, std::ref(filter)));
        for (std::string token : tracer) {
            // just iterate, we're already pulling through tracer
        }
    }

    {
        std::cout << "\nlayout() as coroutine::push_type:\n";
        std::istringstream infile(data);
        coro_t::pull_type reader(std::bind(readlines, _1, std::ref(infile)));
        coro_t::pull_type tokenizer(std::bind(tokenize, _1, std::ref(reader)));
        coro_t::pull_type filter(std::bind(only_words, _1, std::ref(tokenizer)));
        coro_t::push_type writer(std::bind(layout, _1, 5, 15));
        for (std::string token : filter) {
            writer(token);
        }
    }

    {
        std::cout << "\nfiltering output:\n";
        std::istringstream infile(data);
        coro_t::pull_type reader(std::bind(readlines, _1, std::ref(infile)));
        coro_t::pull_type tokenizer(std::bind(tokenize, _1, std::ref(reader)));
        coro_t::push_type writer(std::bind(layout, _1, 5, 15));
        // Because of the symmetry of the API, we can use any of these
        // chaining functions in a push_type coroutine chain as well.
        coro_t::push_type filter(std::bind(only_words, std::ref(writer), _1));
        for (std::string token : tokenizer) {
            filter(token);
        }
    }
}