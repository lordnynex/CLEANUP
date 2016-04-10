#ifndef SMPPGATEFILTERPARSER_H
#define SMPPGATEFILTERPARSER_H

#include <boost/spirit/include/classic.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

#include <map>
#include <list>

#include "Operation.h"

using namespace boost::spirit::classic;

namespace sms {

    class SMPPFilterBuilder {
    public:        
        typedef boost::function<void()> FuncPTR;
        typedef std::map< std::string, boost::any > valsMapT;
        typedef std::list< std::string > funcListT;

        enum OpType {
            OP_STRING,
            OP_NUMBER,
            OP_LINKED_STRING,
            OP_LINKED_NUMBER,
            OP_MARKED_STRING,
            OP_MARKED_NUMBER,
            OP_BOOL
        };

        typedef Operation<
                Vector7<
                std::string,
                long,
                std::string,
                std::string,
                std::string,
                long,
                bool
                >::Type
                > Operand;

        typedef std::list< Operand > opsListT;

        void addRangeArgMark();

        Operand delRangeArgMark( Operand op );

        bool check(valsMapT vals);
    private:
        bool isMarked( Operand op );
        bool isLinked( Operand op );
        void addNumericArg( const char* begin, const char* end );
        void addStringArg( const char* begin, const char* end );
        void addLinkedNumericArg( const char* begin, const char* end );
        void addLinkedStringArg( const char* begin, const char* end );
        void add_AND_Operation();
        void add_OR_Operation();
        void add_NOT_Operation();
        void add_EQUAL_Operation();
        void add_LESS_Operation();
        void add_MORE_Operation();
        void add_EQUAL_OR_LESS_Operation();
        void add_EQUAL_OR_MORE_Operation();
        void add_NOT_EQUAL_Operation();
        void add_IN_Operation();
        void add_NEGATIVE_Operation();
        void add_POSITIVE_Operation();
        opsListT args_link( valsMapT& map, const opsListT& ops );

        funcListT funcList;
        opsListT ops_list;
    };

    using boost::spirit::classic::space_p;

    class SMPPGateFilterGrammar: public grammar<SMPPGateFilterGrammar> {
    public:

        SMPPGateFilterGrammar( SMPPFilterBuilder& _fb ):fb(_fb) {}

        template <typename ScannerT>
        struct definition {
            definition(SMPPGateFilterGrammar const& gr_const) {
                SMPPGateFilterGrammar& gr = const_cast< SMPPGateFilterGrammar& >( gr_const );
                range
                        =   '[' >>
                            *(tier2 >> ';')[boost::bind(&SMPPFilterBuilder::addRangeArgMark, &gr.fb)] >>
                            ']'
                            ;

                tier2
                        =   tier1
                            >> *(   (str_p("AND") >> tier1)[boost::bind(&SMPPFilterBuilder::add_AND_Operation, &gr.fb)]
                                    |   (str_p("OR") >> tier1)[boost::bind(&SMPPFilterBuilder::add_OR_Operation, &gr.fb)]
                                    )
                            ;

                tier1
                        =   tier0
                            >> *(   (str_p("==") >> tier0)[boost::bind(&SMPPFilterBuilder::add_EQUAL_Operation, &gr.fb)]
                                    |   (str_p("<") >> tier0)[boost::bind(&SMPPFilterBuilder::add_LESS_Operation, &gr.fb)]
                                    |   (str_p(">") >> tier0)[boost::bind(&SMPPFilterBuilder::add_MORE_Operation, &gr.fb)]
                                    |   (str_p("<=") >> tier0)[boost::bind(&SMPPFilterBuilder::add_EQUAL_OR_LESS_Operation, &gr.fb)]
                                    |   (str_p(">=") >> tier0)[boost::bind(&SMPPFilterBuilder::add_EQUAL_OR_MORE_Operation, &gr.fb)]
                                    |   (str_p("!=") >> tier0)[boost::bind(&SMPPFilterBuilder::add_NOT_EQUAL_Operation, &gr.fb)]
                                    |   (str_p("IN") >> range)[boost::bind(&SMPPFilterBuilder::add_IN_Operation, &gr.fb)]
                                    )
                            ;

                tier0
                        =   lexeme_d[(+digit_p >> *('.' >> +digit_p))
                                     [boost::bind(&SMPPFilterBuilder::addNumericArg, &gr.fb, _1, _2)]
                                     ]
                            |   lexeme_d['"' >> (+(alnum_p|' '|'.'|':'))
                                         [boost::bind(&SMPPFilterBuilder::addStringArg, &gr.fb, _1, _2)] >> '"']
                            |   str_p("TO")[boost::bind(&SMPPFilterBuilder::addLinkedStringArg, &gr.fb, _1, _2)]
                            |   str_p("COUNTRYCODE")[boost::bind(&SMPPFilterBuilder::addLinkedStringArg, &gr.fb, _1, _2)]
                            |   str_p("COUNTRY")[boost::bind(&SMPPFilterBuilder::addLinkedStringArg, &gr.fb, _1, _2)]
                            |   str_p("OPERATORCODE")[boost::bind(&SMPPFilterBuilder::addLinkedStringArg, &gr.fb, _1, _2)]
                            |   str_p("OPERATOR")[boost::bind(&SMPPFilterBuilder::addLinkedStringArg, &gr.fb, _1, _2)]
                            |   str_p("FROM")[boost::bind(&SMPPFilterBuilder::addLinkedStringArg, &gr.fb, _1, _2)]
                            |   str_p("RQS")[boost::bind(&SMPPFilterBuilder::addLinkedNumericArg, &gr.fb, _1, _2)]
                            |   str_p("RQM")[boost::bind(&SMPPFilterBuilder::addLinkedNumericArg, &gr.fb, _1, _2)]
                            |   str_p("RQ5M")[boost::bind(&SMPPFilterBuilder::addLinkedNumericArg, &gr.fb, _1, _2)]
                            |   str_p("RQD")[boost::bind(&SMPPFilterBuilder::addLinkedNumericArg, &gr.fb, _1, _2)]
                            |   str_p("RQ2D")[boost::bind(&SMPPFilterBuilder::addLinkedNumericArg, &gr.fb, _1, _2)]
                            |   str_p("RQMo")[boost::bind(&SMPPFilterBuilder::addLinkedNumericArg, &gr.fb, _1, _2)]
                            |   '(' >> tier2 >> ')'
                            |   (str_p("NOT") >> tier2)[boost::bind(&SMPPFilterBuilder::add_NOT_Operation, &gr.fb)]                            |   ('-' >> tier0)[boost::bind(&SMPPFilterBuilder::add_NEGATIVE_Operation, &gr.fb)]
                            |   ('+' >> tier0)[boost::bind(&SMPPFilterBuilder::add_POSITIVE_Operation, &gr.fb)]
                            ;
            }

            rule<ScannerT> range, tier0, tier1, tier2;

            rule<ScannerT> const&
                    start() const { return tier2; }
        };
    private:
        SMPPFilterBuilder& fb;
    };

    class SMPPGateFilterParser {
    public:
        struct ResT {
            bool ok;
            std::string where;
            SMPPFilterBuilder filter;
        };

        ResT parseStr( std::string req ) {
            ResT r;
            SMPPGateFilterGrammar gr( r.filter );
            parse_info<> info = parse( req.c_str(), gr, space_p );
            r.ok = info.full;
            r.where = info.stop;

            return r;
        }
    };
}

#endif // SMPPGATEFILTERPARSER_H
