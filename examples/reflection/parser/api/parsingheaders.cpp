#include <siplasplas/jobs/engine.hpp>
#include <siplasplas/reflection/parser/api/core/clang/index.hpp>
#include <siplasplas/constexpr/arrayview.hpp>

using namespace cpp::jobs;
using namespace cpp::reflection::parser::api::core::clang;

class Parser
{
public:
    Parser(const std::vector<std::string>& files, std::size_t threads) :
        _files{files},
        _engine{threads, 1024}
    {}

    void run()
    {
        Job* compileFiles = _engine.threadWorker()->pool().createClosureJob(
            [this](Job& job)
            {
                for(const auto& file : _files)
                {
                    std::cout << "Parser::run(): " << file << std::endl;

                    parseFile(file, job);
                }
            }
        );

        _engine.threadWorker()->submit(compileFiles);
        _engine.threadWorker()->wait(compileFiles);
    }

    TranslationUnit parseFile(const std::string& file)
    {
        std::cout << "Parser::parseFile(\"" << file << "\")" << std::endl;

        return _index.parse(file, CompileOptions()
            .std("c++14")
            .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
            .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
            .I(SIPLASPLAS_INCLUDE_DIR)
            .I(SIPLASPLAS_REFLECTION_OUTPUT_DIR)
            .I(SIPLASPLAS_EXPORTS_DIR)
            .I(CTTI_INCLUDE_DIR)
        );
    }

    void parseFile(const std::string& file, Job& parentJob)
    {
        Job* fileJob = _engine.threadWorker()->pool().createClosureJobAsChild(
            [this, file, &parentJob](Job& fileJob)
            {
                if(!alreadyParsed(file))
                {
                    auto tu = parseFile(file);
                    auto inclusions = tu.inclusions();
                    storeTu(file, tu);

                    for(const auto& inclusion : inclusions)
                    {
                        parseFile(inclusion.file().fileName().str().str(), fileJob);
                    }
                }
                else
                {
                    std::cout << "Parser::parseFile(\"" << file << "\"): Already parsed, skipping" << std::endl;
                }
            },
            &parentJob
        );

        if(fileJob)
        {
            _engine.threadWorker()->submit(fileJob);
        }
    }

    const std::unordered_map<std::string, TranslationUnit>& translationUnits() const
    {
        return _tus;
    }
private:
    Index _index;
    std::vector<std::string> _files;
    mutable std::recursive_mutex _lockTus;
    std::unordered_map<std::string, TranslationUnit> _tus;
    Engine _engine;

    bool alreadyParsed(const std::string& file) const
    {
        std::lock_guard<std::recursive_mutex> guard{_lockTus};
        return _tus.find(file) != _tus.end();
    }

    void storeTu(const std::string& file, TranslationUnit& tu)
    {
        std::lock_guard<std::recursive_mutex> guard{_lockTus};

        if(!alreadyParsed(file))
        {
            _tus.emplace(file, std::move(tu));
        }
        else
        {
            std::cout << "Parser::storeTu(\"" << file << "\"): Already parsed, will be ignored..." << std::endl;
        }
    }
};

int main()
{
    Parser parser{{
        SIPLASPLAS_INCLUDE_DIR "/siplasplas/reflection/parser/api/core/clang/index.hpp"
    }, 4};
    parser.run();

    for(const auto& keyValue : parser.translationUnits())
    {
        std::cout << "TranslationUnit: " << keyValue.second.spelling() << std::endl;
    }
}
