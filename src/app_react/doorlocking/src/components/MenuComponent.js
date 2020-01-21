import React, { Component } from "react";
import {Link} from 'react-router-dom'
import PropTypes from 'prop-types';

/**
 * Application sidebar menu module.
 * 
 * @module MenuComponent
 */


/**
 * Object with the properties needed to render a menu item.
 * 
 * @typedef {Object} MenuLineItem
 * @property {string} name - The name of the menu to be shown
 * @property {string} link - The link of the menu item
 */

/**
 * Menu props.
 * 
 * @typedef {Object} MenuProps
 * @property {Object} layout - reference to the div that encloses the menu
 * @property {Array.<MenuLineItem>} menus - Menus to be displayed
 * @property {string} heading - Name to be displayed on the top of the menu
 */

/**
 * Application sidebar menu component.
 * 
 * @author Diogo S. Mendonça
 * @example
 * const layout = React.createRef();
 * const menus = [
                    {link: '/open_door', name: 'Open Door'},
                    {link: '/register_user', name: 'Register User'},
                    {link: '/list_users', name: 'List/Change Users'},
                    {link: '/config', name: 'Configuration'},
                    {link: '/logout_handler', name: 'Logout'}
                  ]
 *
 * const heading = "DoorLocking.App";
 * return (
 *   <BrowserRouter>
 *      <div id="layout" ref={layout}>
 *          <Menu layout={layout} menus={menus} heading={heading} />
 *      </div>
 *   </BrowserRouter>
 * )
 *  
 */
class Menu extends Component {

    

    /**
     * @param {MenuProps} props {layout: element, menus: [{name: 'name', link: 'link}], heading: 'Name of heading'} 
     * 
     * element: the element that suround the menu.
     * menus: array of objects that defines names and links for the menu
     * heading: text to be displayed on the top of the menu
     *  @see {@link module:MenuComponent~MenuProps} 
     *  @see {@link module:MenuComponent~MenuLineItem} 
     */
    constructor(props){
        super(props);
        this._menu = React.createRef();
        this._menuLink = React.createRef();
        this.state = {
            active: false //whether the menu sidebar is displayed
        };
    }

    /**
     * Hides the menu sidebar. Closes the menu if it is opened 
     * in small screen sizes.
     */
    hideMenu(){
        if (this.state.active) {
            this.toggleMenu();
        }
    }
    
    /**
     * Toggle a CSS class in a DOMElement. Include the class if 
     * it is not present. If the class is present in the DOMElement,
     * removes it.
     * 
     * @param {DOMElement} element element to have the CSS class toggled
     * @param {String} className CSS class to be toggled
     */
    _toggleClass(element, className) {
        var classes = element.className.split(/\s+/),
            length = classes.length,
            i = 0;

        for(; i < length; i++) {
          if (classes[i] === className) {
            classes.splice(i, 1);
            break;
          }
        }
        // The className is not found
        if (length === classes.length) {
            classes.push(className);
        }

        element.className = classes.join(' ');
    }

    /**
     * Change the state of the menu by toggling the active CSS class of all DOMElements 
     * needed to show or hide the sidebar menu in small screens.
     * In case, the menu and menuLink DOMElements stored as references
     * to the nodes, and layout element, which the reference is passed as props.
     */
    toggleMenu() {
        var active = 'active';
        this.setState({active: !this.state.active});
        //e.preventDefault();
        this._toggleClass(this.props.layout.current, active);
        this._toggleClass(this._menu.current, active);
        this._toggleClass(this._menuLink.current, active);
    }

    /**
     * Render a line entry of the menu.
     * 
     * @param {Object} menu menu to be displayed {name: 'name', link: 'link}
     * @param {Number} index the index of the menu to generate its id 'menu-index'
     */
    _renderMenuLine(menu, index){
        return(<li className="pure-menu-item" key={'menu-li-' + index}>
                    <Link onClick={(e) => this.hideMenu(e)} 
                            to={menu.link} id={'menu-' + index} key={'menu-link-' + index}
                            className="pure-menu-link">
                                {menu.name}
                    </Link>
                </li>);
    }

    /**
     * Presents the menu sidebar.
     * Include Link and onClick event handlers to toggle the menu sidebar and to access
     *  the functionalities of the system.
     */
    render(){
        return(
            <>
                <a href="#menu" ref={this._menuLink} id="menuLink" className="menu-link" onClick={(e) => this.toggleMenu(e)}>
                    <span></span>
                </a>

                <div id="menu" ref={this._menu}>
                    <div className="pure-menu">
                        <a className="pure-menu-heading" href="/">{this.props.heading}</a>
                        <ul className="pure-menu-list">
                            {this.props.menus.map((menu, index)=>this._renderMenuLine(menu, index))}
                        </ul>
                    </div>
                </div>
            </>
        );
    }
}

Menu.propTypes = {
    //div that encloses the menu
    layout: PropTypes.object.isRequired,
    //menus to be displayed {name: 'name', link: 'link}
    menus: PropTypes.arrayOf(PropTypes.object).isRequired,
    //Name to be displayed on the top of the menu
    heading: PropTypes.string.isRequired
}

export default Menu;