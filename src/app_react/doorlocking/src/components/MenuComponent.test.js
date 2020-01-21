import React from 'react';
import Menu from './MenuComponent';
import {BrowserRouter} from 'react-router-dom';
import { shallow, mount, render } from 'enzyme';


describe('<Menu />', () => {

    var layout = React.createRef();
    
    const menus = [
                    {link: '/open_door', name: 'Open Door'},
                    {link: '/register_user', name: 'Register User'},
                    {link: '/list_users', name: 'List/Change Users'},
                    {link: '/config', name: 'Configuration'},
                    {link: '/logout_handler', name: 'Logout'}
                  ]

    const heading = "DoorLocking.App";
        
    const menu = mount(
        <BrowserRouter>
            <div id="layout" ref={layout}>
                <Menu layout={layout} menus={menus} heading={heading} />
            </div>
        </BrowserRouter>  
    );

    /**
     * aceptance test
     */
    it('renders without crashing', () => {
        
        //check whether the html elements where created 
        expect(menu.find('#layout').exists()).toEqual(true);
        expect(menu.find('#menuLink').exists()).toEqual(true);
        expect(menu.find('#menu').exists()).toEqual(true);

    });

    /**
     * aceptance test
     */
    it('clicks on menu hamburger link and menu open', () => {
        //pre-condition
        expect(menu.find('#layout').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menuLink').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menu').render().hasClass("active")).toEqual(false);
        expect(menu.find(Menu).state('active')).toEqual(false);
        //event
        menu.find('#menuLink').simulate('click');
        //post-condition
        expect(menu.find('#layout').render().hasClass("active")).toEqual(true);
        expect(menu.find('#menuLink').render().hasClass("active")).toEqual(true);
        expect(menu.find('#menu').render().hasClass("active")).toEqual(true);
        expect(menu.find(Menu).state('active')).toEqual(true);
    })

    /**
     * aceptance test
     */
    it('clicks on menu item link and menu close', () => {
        //pre-condition
        expect(menu.find('#layout').render().hasClass("active")).toEqual(true);
        expect(menu.find('#menuLink').render().hasClass("active")).toEqual(true);
        expect(menu.find('#menu').render().hasClass("active")).toEqual(true);
        expect(menu.find(Menu).state('active')).toEqual(true);

        //event
        menu.find('a#menu-1').simulate('click');

        //post-condition
        expect(menu.find('#layout').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menuLink').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menu').render().hasClass("active")).toEqual(false);
        expect(menu.find(Menu).state('active')).toEqual(false);
    })

    /**
     * defect test
     */
    it('clicks on menu item link when it is not active and menu do not open', () => {
        //pre-condition
        expect(menu.find('#layout').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menuLink').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menu').render().hasClass("active")).toEqual(false);
        expect(menu.find(Menu).state('active')).toEqual(false);

        //event
        menu.find('a#menu-1').simulate('click');

        //post-condition
        expect(menu.find('#layout').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menuLink').render().hasClass("active")).toEqual(false);
        expect(menu.find('#menu').render().hasClass("active")).toEqual(false);
        expect(menu.find(Menu).state('active')).toEqual(false);
    })

});
