#ifndef TARIFFOPTIONEDITOR_H
#define TARIFFOPTIONEDITOR_H

#include "Tariff.h"

#include <Wt/WContainerWidget>
#include <Wt/WApplication>
#include <Wt/WCheckBox>
#include <Wt/WString>
#include <Wt/WGridLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>

template < class Option, class DisplayAdaptor >
class TariffOptionEditor: public Wt::WGridLayout, public DisplayAdaptor {
public:
    TariffOptionEditor( Tariff* _tariff, Wt::WContainerWidget* parent = 0 ):
                DisplayAdaptor( _tariff ),
                Wt::WGridLayout( parent ),
                tariff( _tariff ) {

        optionState = new Wt::WCheckBox( Wt::WString::fromUTF8( Option::getName() ) );
        optionState->setTristate();
        optionState->changed().connect( boost::bind( &TariffOptionEditor< Option, DisplayAdaptor >::optionChanged, this ) );
        optionState_canBeUnchecked = true;

        Wt::WVBoxLayout* opt_box_layout = new Wt::WVBoxLayout();
        DisplayAdaptor::buildValuesGroup( opt_box_layout );
        opt_box_layout->setSpacing( 1 );

        Wt::WGridLayout::setVerticalSpacing( 1 );
        addWidget( optionState, 0, 0, 1, 1, Wt::AlignLeft );
        addItem( opt_box_layout, 1, 0, 1, 1, Wt::AlignRight );

        repaintOption();
    }

    virtual void repaintOption() {
        boost::logic::tribool opt_exists;
        Option option;

        switch ( DisplayAdaptor::last_post_type ) {
        case DisplayAdaptor::POS_ROOT:
            opt_exists = tariff->hasOption< Option >();
            option = tariff->getOption< Option >();
            break;
        case DisplayAdaptor::POS_COUNTRY:
            opt_exists = tariff->hasOption< Option >( DisplayAdaptor::last_mcc );
            option = tariff->getOption< Option >( DisplayAdaptor::last_mcc );
            break;
        case DisplayAdaptor::POS_OPERATOR:
            opt_exists = tariff->hasOption< Option >( DisplayAdaptor::last_mcc, DisplayAdaptor::last_mnc );
            option = tariff->getOption< Option >( DisplayAdaptor::last_mcc, DisplayAdaptor::last_mnc );
            break;
        }

        if ( opt_exists ) {
            optionState->setCheckState( Wt::Checked );
            optionState_canBeUnchecked = true;
            DisplayAdaptor::setVisible( true );
            DisplayAdaptor::setDisabled( false );

        } else if ( !opt_exists ) {
            optionState->setCheckState( Wt::Unchecked );
            optionState_canBeUnchecked = true;
            DisplayAdaptor::setVisible( false );
            DisplayAdaptor::setDisabled( true );
        } else {
            optionState->setCheckState( Wt::PartiallyChecked );
            optionState_canBeUnchecked = false;
            DisplayAdaptor::setVisible( true );
            DisplayAdaptor::setDisabled( true );
        }

        DisplayAdaptor::repaintValues();
    }

    void optionChanged() {
        Wt::WApplication::instance()->processEvents();
        if ( !optionState_canBeUnchecked )
            if ( optionState->checkState() == Wt::Unchecked ) {
                optionState->setChecked( true );
                Wt::WApplication::instance()->processEvents();
            }

        if ( optionState->checkState() == Wt::Unchecked ) {
            boost::logic::tribool opt_exists;

            switch ( DisplayAdaptor::last_post_type ) {
            case DisplayAdaptor::POS_ROOT:
                opt_exists = tariff->hasOption< Option >();
                break;
            case DisplayAdaptor::POS_COUNTRY:
                opt_exists = tariff->hasOption< Option >( DisplayAdaptor::last_mcc );
                break;
            case DisplayAdaptor::POS_OPERATOR:
                opt_exists = tariff->hasOption< Option >( DisplayAdaptor::last_mcc, DisplayAdaptor::last_mnc );
                break;
            }

            if ( opt_exists ) {
                switch ( DisplayAdaptor::last_post_type ) {
                case DisplayAdaptor::POS_ROOT:
                    tariff->removeOption< Option >();
                    break;
                case DisplayAdaptor::POS_COUNTRY:
                    tariff->removeOption< Option >( DisplayAdaptor::last_mcc );
                    break;
                case DisplayAdaptor::POS_OPERATOR:
                    tariff->removeOption< Option >( DisplayAdaptor::last_mcc, DisplayAdaptor::last_mnc );
                    break;
                }
            }

        }

        if ( optionState->checkState() == Wt::Checked ) {
            boost::logic::tribool opt_exists;

            switch ( DisplayAdaptor::last_post_type ) {
            case DisplayAdaptor::POS_ROOT:
                opt_exists = tariff->hasOption< Option >();
                break;
            case DisplayAdaptor::POS_COUNTRY:
                opt_exists = tariff->hasOption< Option >( DisplayAdaptor::last_mcc );
                break;
            case DisplayAdaptor::POS_OPERATOR:
                opt_exists = tariff->hasOption< Option >( DisplayAdaptor::last_mcc, DisplayAdaptor::last_mnc );
                break;
            }

            if ( !opt_exists || boost::logic::indeterminate( opt_exists ) ) {
                switch ( DisplayAdaptor::last_post_type ) {
                case DisplayAdaptor::POS_ROOT:
                    tariff->setOption< Option >( Option() );
                    break;
                case DisplayAdaptor::POS_COUNTRY:
                    tariff->setOption< Option >( Option(), DisplayAdaptor::last_mcc );
                    break;
                case DisplayAdaptor::POS_OPERATOR:
                    tariff->setOption< Option >( Option(), DisplayAdaptor::last_mcc, DisplayAdaptor::last_mnc );
                    break;
                }
            }

        }

        repaintOption();
    }

private:
    Tariff* tariff;
    Wt::WCheckBox* optionState;
    bool optionState_canBeUnchecked;
    Wt::WGridLayout* opt_box_layout;
};

class AbstractDisplayAdaptor {
public:
    void setCurPosRoot( ) {
        last_post_type = POS_ROOT;
        last_mcc = "";
        last_mnc = "";

        repaintOption();
    }

    void setCurPosCountry( std::string mcc ) {
        last_post_type = POS_COUNTRY;
        last_mcc = mcc;
        last_mnc = "";

        repaintOption();
    }

    void setCurPosOperator( std::string mcc, std::string mnc ) {
        last_post_type = POS_OPERATOR;
        last_mcc = mcc;
        last_mnc = mnc;

        repaintOption();
    }

protected:
    enum POSITION_TYPE {
        POS_ROOT,
        POS_COUNTRY,
        POS_OPERATOR
    };

    virtual void repaintOption() = 0;

    POSITION_TYPE last_post_type;
    std::string last_mcc;
    std::string last_mnc;

};

template < class Option >
class MultiValueAdaptor: public AbstractDisplayAdaptor {
public:
    MultiValueAdaptor( Tariff* _tariff ): tariff( _tariff ) {}

    template < class Container >
    void buildValuesGroup( Container* opt_box_grp ) {
        typename Option::DescriptionList description = Option::getDescriptions();
        int layout_counter = 0;
        for ( typename Option::DescriptionList::iterator it = description.begin(); it != description.end(); it++, layout_counter++ ) {
            Wt::WCheckBox* opt_box = new Wt::WCheckBox( Wt::WString::fromUTF8( *it ) );
            opt_box->changed().connect( boost::bind( &MultiValueAdaptor< Option >::valueChanged, this, *it ) );

            opt_box_grp->addWidget( opt_box );
            checkboxes.insert( std::make_pair( *it, opt_box ) );
        }
    }

    void setDisabled( bool dis ) {
        for ( std::map< std::string, Wt::WCheckBox* >::iterator it = checkboxes.begin(); it != checkboxes.end(); it++) {
            it->second->setDisabled( dis );
        }
    }

    void setVisible( bool vis ) {
        for ( std::map< std::string, Wt::WCheckBox* >::iterator it = checkboxes.begin(); it != checkboxes.end(); it++) {
            if ( vis )
                it->second->show();
            else
                it->second->hide();
        }
    }

    void repaintValues() {

        boost::logic::tribool opt_exists;
        Option option;

        switch ( last_post_type ) {
        case POS_ROOT:
            opt_exists = tariff->hasOption< Option >();
            option = tariff->getOption< Option >();
            break;
        case POS_COUNTRY:
            opt_exists = tariff->hasOption< Option >( last_mcc );
            option = tariff->getOption< Option >( last_mcc );
            break;
        case POS_OPERATOR:
            opt_exists = tariff->hasOption< Option >( last_mcc, last_mnc );
            option = tariff->getOption< Option >( last_mcc, last_mnc );
            break;
        }

        if ( opt_exists || boost::logic::indeterminate( opt_exists )) {

            for ( std::map< std::string, Wt::WCheckBox* >::iterator it = checkboxes.begin(); it != checkboxes.end(); it++) {
                it->second->setCheckState( Wt::Unchecked );
            }

            typename Option::ValuesListT values = option.getValues();
            for ( typename Option::ValuesListT::iterator it = values.begin(); it != values.end(); it++) {
                checkboxes[*it]->setCheckState( Wt::Checked );
            }
        }
    }



    void valueChanged( std::string name ) {
        Wt::WApplication::instance()->processEvents();
        Wt::WCheckBox* chk_box = checkboxes[ name ];
        Option option;
        switch ( last_post_type ) {
        case POS_ROOT:
            option = tariff->getOption< Option >();

            if ( chk_box->isChecked() ) {
                typename Option::ValuesListT values;
                values = option.getValues();
                values.insert( name );
                option.setValues( values );
            }
            else {
                typename Option::ValuesListT values;
                values = option.getValues();
                values.erase( name );
                option.setValues( values );
            }

            tariff->setOption<Option>( option );
            break;
        case POS_COUNTRY:
            option = tariff->getOption< Option >( last_mcc );

            if ( chk_box->isChecked() ) {
                typename Option::ValuesListT values;
                values = option.getValues();
                values.insert( name );
                option.setValues( values );
            }
            else {
                typename Option::ValuesListT values;
                values = option.getValues();
                values.erase( name );
                option.setValues( values );
            }

            tariff->setOption<Option>( option, last_mcc );
            break;
        case POS_OPERATOR:
            option = tariff->getOption< Option >( last_mcc, last_mnc );

            if ( chk_box->isChecked() ) {
                typename Option::ValuesListT values;
                values = option.getValues();
                values.insert( name );
                option.setValues( values );
            }
            else {
                typename Option::ValuesListT values;
                values = option.getValues();
                values.erase( name );
                option.setValues( values );
            }

            tariff->setOption<Option>( option, last_mcc, last_mnc );
            break;
        }
        repaintValues();
    }

private:
    Tariff* tariff;
    std::map< std::string, Wt::WCheckBox* > checkboxes;
};

template < class Option >
class MultiChoiseAdaptor: public AbstractDisplayAdaptor {
public:
    MultiChoiseAdaptor( Tariff* _tariff ): tariff( _tariff ) {}

    template < class Container >
    void buildValuesGroup( Container* opt_box_grp ) {
        typename Option::DescriptionList description = Option::getDescriptions();
        int layout_counter = 0;
        for ( typename Option::DescriptionList::iterator it = description.begin(); it != description.end(); it++, layout_counter++ ) {
            Wt::WRadioButton* opt_box = new Wt::WRadioButton( Wt::WString::fromUTF8( *it ) );
            opt_box->changed().connect( boost::bind( &MultiChoiseAdaptor< Option >::valueChanged, this, *it ) );

            opt_box_grp->addWidget( opt_box );
            checkboxes.insert( std::make_pair( *it, opt_box ) );
        }
    }

    void setDisabled( bool dis ) {
        for ( std::map< std::string, Wt::WRadioButton* >::iterator it = checkboxes.begin(); it != checkboxes.end(); it++) {
            it->second->setDisabled( dis );
        }
    }

    void setVisible( bool vis ) {
        for ( std::map< std::string, Wt::WRadioButton * >::iterator it = checkboxes.begin(); it != checkboxes.end(); it++) {
            if ( vis )
                it->second->show();
            else
                it->second->hide();
        }
    }

    void repaintValues() {

        boost::logic::tribool opt_exists;
        Option option;

        switch ( last_post_type ) {
        case POS_ROOT:
            opt_exists = tariff->hasOption< Option >();
            option = tariff->getOption< Option >();
            break;
        case POS_COUNTRY:
            opt_exists = tariff->hasOption< Option >( last_mcc );
            option = tariff->getOption< Option >( last_mcc );
            break;
        case POS_OPERATOR:
            opt_exists = tariff->hasOption< Option >( last_mcc, last_mnc );
            option = tariff->getOption< Option >( last_mcc, last_mnc );
            break;
        }

        if ( opt_exists || boost::logic::indeterminate( opt_exists )) {

            for ( std::map< std::string, Wt::WRadioButton* >::iterator it = checkboxes.begin(); it != checkboxes.end(); it++) {
                    it->second->setChecked( false );
            }

            checkboxes[ option.getValue() ]->setChecked( true );
        }
    }

    void valueChanged( std::string name ) {
        Wt::WApplication::instance()->processEvents();
        Wt::WRadioButton* chk_box = checkboxes[ name ];
        Option option;
        switch ( last_post_type ) {
        case POS_ROOT:
            option = tariff->getOption< Option >();

            if ( chk_box->isChecked() ) {
                option.setValue( name );
            }

            tariff->setOption<Option>( option );
            break;
        case POS_COUNTRY:
            option = tariff->getOption< Option >( last_mcc );

            if ( chk_box->isChecked() ) {
                option.setValue( name );
            }

            tariff->setOption<Option>( option, last_mcc );
            break;
        case POS_OPERATOR:
            option = tariff->getOption< Option >( last_mcc, last_mnc );

            if ( chk_box->isChecked() ) {
                option.setValue( name );
            }

            tariff->setOption<Option>( option, last_mcc, last_mnc );
            break;
        }
        repaintValues();
    }

private:
    Tariff* tariff;
    std::map< std::string, Wt::WRadioButton* > checkboxes;
};


#endif // TARIFFOPTIONEDITOR_H
