import clang.cindex
import asciitree
from utility.logger import GlobalLogger
from ast.translationunit import TranslationUnit

class TranslationUnitProcessor:
    """ Processes a translation unit file (Usually a .cpp, but an isolated header works too)
        generating its AST. Also implements reflection code generation on top of that AST.
    """

    def __init__(self, filepath, args, jinjaTemplate, logger, print_ast = False):
        """ Sets up a translation unit processor, given the file to parse, compiler args,
            the code generation template, etc
        """

        self.logger = logger
        self.filePath = filepath
        self.compileArgs = args
        self.ast_file_path = filepath + '.ast'
        self.print_ast = print_ast
        self.index = clang.cindex.Index.create()
        self.jinjaTemplate = jinjaTemplate

        self.classes = []
        self.root = None

    def process(self):
        """ Process the translation unit

            Generates an ast.TranslationUnit instance with all the
            translation unit data (AST, macro instances, etc).
            Also sets up the needed data for reflection code generation
            (See runJinja() method below)
        """

        self.logger.info('Parsing file...')

        ast_options = clang.cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD
        self.clang_tu = self.index.parse(self.filePath, args = self.compileArgs, options = ast_options)

        for d in self.clang_tu.diagnostics:
            GlobalLogger.error().step('Line {} (severity {}): {}'.format(d.location.line, d.severity, d.spelling))

        self.logger.info('Processing AST...')

        self.translation_unit = TranslationUnit(self.clang_tu.cursor, self.filePath)

        # This is the root of the AST. For easy visitation, check TranslationUnit.nodes() method,
        # it gives an iterable on the translation unit AST
        self.root = self.translation_unit.root

        # Print the processed AST and the full AST given by libclang.
        if self.print_ast:
            self.logger.info('Dumping AST to {}...'.format(self.ast_file_path))
            with open(self.ast_file_path, 'w') as ast_file:
                import asciitree

                ast_file.write(asciitree.draw_tree(self.root,
                    lambda n: list(n.get_children()),
                    lambda n: n.print_ast_node()
                ))

                ast_file.write('\n\n\nFULL AST BELLOW:\n\n\n')

                ast_file.write(asciitree.draw_tree(self.clang_tu.cursor,
                    lambda c: list(c.get_children()),
                    lambda c: 'File {}, line {}: \'{}\', {}'.format(c.location.file, c.location.line, c.displayname or c.spelling, c.kind)
                ))


    def dispose(self):
        self.clang_tu = None
        self.translation_unit = None
        self.root = None


    def run_jinja(self, outputfile):
        """ Generate reflection code for the translation unit"""

        if self.jinjaTemplate:
            import hashlib

            self.logger.info('Generating file...')

            with open(outputfile, 'w') as outputFile:
                outputFile.write(self.jinjaTemplate.render(global_namespace = self.translation_unit.root, hash = hashlib.md5(outputfile.encode()).hexdigest()))


