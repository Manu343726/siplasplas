#include <siplasplas/jobs/engine.hpp>
#include <siplasplas/reflection/parser/api/core/clang/index.hpp>

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
        std::unordered_map<std::string, TranslationUnit> tus;
        tus.reserve(_files.size());

        Job* compileFiles = _engine.threadWorker()->pool().createClosureJob(
            [this, &tus](Job& job)
            {
                for(const auto& file : _files)
                {
                    parseFile(file, tus, job);
                }
            }
        );

        _engine.threadWorker()->submit(compileFiles);
        _engine.threadWorker()->wait(compileFiles);
    }

    TranslationUnit parseFile(const std::string& file)
    {
        return _index.parse(file, CompileOptions()
            .std("c++11")
            .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
            .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
        );
    }

    void parseFile(const std::string& file, std::unordered_map<std::string, TranslationUnit>& tus, Job& parentJob)
    {
        Job* fileJob = _engine.threadWorker()->pool().createClosureJobAsChild(
            [this, file, &tus, &parentJob](Job& fileJob)
            {
                auto tu = parseFile(file);
                auto inclusions = tu.inclusions();
                std::unordered_map<std::string, TranslationUnit> tus;
                tus.reserve(inclusions.size());

                for(const auto& inclusion : inclusions)
                {
                    parseFile(inclusion.file().fileName().str().str(), tus, fileJob);
                }
            },
            &parentJob
        );

        _engine.threadWorker()->submit(fileJob);
    }

private:
    Index _index;
    std::vector<std::string> _files;
    Engine _engine;
};

int main()
{
    Parser parser{{"../../astexamples.hpp", "../../myclass.hpp", "../../myenum.hpp"}, 4};
    parser.run();
}
