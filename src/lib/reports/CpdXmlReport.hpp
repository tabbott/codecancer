#pragma once

#include "ReportBase.hpp"

#include <rapidxml.hpp>
#include <boost/lexical_cast.hpp>

#include <ostream>
#include <string>

class EsaView;
class LcpInterval;

class CpdXmlReport : public ReportBase {
public:
    CpdXmlReport(EsaView const& esaView, std::ostream& out);

    void observe(LcpInterval const& interval);
    void finalize();

private:
    EsaView const& _esaView;
    std::ostream& _out;
    rapidxml::xml_document<> _doc;
    rapidxml::xml_node<>* _docRoot;

private:
    template<typename Node, typename T>
    void add_attribute(Node* node, char const* name, T const& value) {
        std::string valueStr = boost::lexical_cast<std::string>(value);
        auto attr = _doc.allocate_attribute(name, valueStr.data());
        node->append_attribute(attr);
    }
};
